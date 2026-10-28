/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_partition.h"
#include "esp_http_client.h"

#include "esp_at.h"
#include "esp_at_internal.h"

#ifdef CONFIG_AT_OTA_SUPPORT
#include "at_ota.h"
#include "esp_at_ota.h"

typedef enum {
    AT_UPGRADE_SYSTEM_FIRMWARE = 0,         /**< upgrade type is system firmware */
    AT_UPGRADE_CUSTOM_PARTITION,            /**< upgrade type is custom partition */
} at_upgrade_type_t;

typedef enum {
    AT_OTA_STATE_IDLE = 0,                  /**< not in upgrade */
    AT_OTA_STATE_FOUND_SERVER,              /**< the upgrade server found */
    AT_OTA_STATE_CONNECTED_TO_SERVER,       /**< connected to the server */
    AT_OTA_STATE_GOT_VERSION,               /**< got the version information to be upgraded */
    AT_OTA_STATE_DONE,                      /**< upgrade succeeded */
    AT_OTA_STATE_FAILED = -1,               /**< upgrade failed */
} at_ota_state_t;

/* One upgrade stops at this deadline. Checked before each send and receive, and before commit. */
#define AT_OTA_TIMEOUT_MS                   (3 * 60 * 1000)
/* No data for this long aborts the upgrade. One wait is also capped by the time left. */
#define AT_OTA_HTTP_TIMEOUT_MS              10000
#define AT_OTA_RX_BUF_SIZE                  1024
#define AT_OTA_QUERY_BUF_SIZE               192
#define AT_OTA_NONBLOCKING_MODE_MAX         2
#define AT_PARTITION_MAGIC_LEN_MAX          3
#define AT_OTA_VERSION_LEN_MAX              64
#define AT_OTA_PARTITION_NAME_LEN_MAX       64
#define AT_OTA_NB_TASK_STACK_SIZE           5120

#define AT_OTA_ROM_PATH                     "/v1/device/rom/"
/* Version JSON is chunked and has no Content-Length. One byte is the NUL for strstr. */
#define AT_OTA_FWVER_BODY_MAX               8192
#define AT_OTA_VERSION_QUERY                "is_format_simple=true"
#define AT_OTA_VERSION_MARK                 "rom_version\": \""

typedef struct at_partition_sig {
    const char *name;
    uint32_t len;
    uint32_t offset;
    uint8_t magic[AT_PARTITION_MAGIC_LEN_MAX];
} at_partition_sig_t;

static const at_partition_sig_t s_at_partition_sig[] = {
#if defined(CONFIG_BOOTLOADER_COMPRESSED_ENABLED) && defined(CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT)
    {"ota", 3, 0, {0x45, 0x53, 0x50}},
#else
    {"ota", 1, 0, {0xE9}},
#endif
    {"mqtt_ca", 2, 0, {0xF1, 0xF1}},
    {"mqtt_cert", 2, 0, {0xF1, 0xF1}},
    {"mqtt_key", 2, 0, {0xF1, 0xF1}},
    {"ble_data", 2, 2, {0x27, 0x95}},
    {"client_ca", 2, 0, {0xF1, 0xF1}},
    {"client_cert", 2, 0, {0xF1, 0xF1}},
    {"client_key", 2, 0, {0xF1, 0xF1}},
    {"server_ca", 2, 0, {0xF1, 0xF1}},
    {"server_cert", 2, 0, {0xF1, 0xF1}},
    {"server_key", 2, 0, {0xF1, 0xF1}},
#ifdef CONFIG_AT_EAP_LEGACY_NAMESPACE_SUPPORT
    {"wpa2_ca", 2, 0, {0xF1, 0xF1}},
    {"wpa2_cert", 2, 0, {0xF1, 0xF1}},
    {"wpa2_key", 2, 0, {0xF1, 0xF1}},
#else
    {"wpa_ent_ca", 2, 0, {0xF1, 0xF1}},
    {"wpa_ent_cert", 2, 0, {0xF1, 0xF1}},
    {"wpa_ent_key", 2, 0, {0xF1, 0xF1}},
#endif
    {"factory_param", 2, 0, {0xFC, 0xFC}},
};

typedef struct {
    int32_t ota_mode;
    char version[AT_OTA_VERSION_LEN_MAX + 1];
    char partition_name[AT_OTA_PARTITION_NAME_LEN_MAX + 1];
} at_ota_param_t;

typedef struct {
    const char *host;
    int port;
    esp_http_client_transport_t transport;
    const char *token;
    /* NUL-terminated PEM. Owned by the upgrade until every client is cleaned up. */
    char *ca_pem;
} at_ota_server_t;

typedef struct {
    at_upgrade_type_t type;
    esp_at_ota_handle_t ota;
    const esp_partition_t *partition;
} at_ota_target_t;

static bool s_ota_started = false;
static at_ota_state_t s_ota_status = AT_OTA_STATE_IDLE;
static TickType_t s_ota_deadline = 0;
/* Event callback cannot see the caller's stack. Set for the client that emits +CIPUPDATE:2. */
static bool s_ota_report_connected = false;

static const char *TAG = "at-ota";

static void at_ota_set_state(at_ota_state_t state)
{
    s_ota_status = state;
}

static void at_ota_report(at_ota_state_t state)
{
    char line[24];
    int len = snprintf(line, sizeof(line), "+CIPUPDATE:%d\r\n", (int)state);
    at_ota_set_state(state);
    if (len > 0 && len < (int)sizeof(line)) {
        esp_at_port_write_data((uint8_t *)line, len);
    }
}

static void at_ota_timeout_start(void)
{
    s_ota_deadline = xTaskGetTickCount() + pdMS_TO_TICKS(AT_OTA_TIMEOUT_MS);
}

static int at_ota_timeout_remain_ms(void)
{
    int32_t left = (int32_t)(s_ota_deadline - xTaskGetTickCount());
    if (left <= 0) {
        return 0;
    }
    return (int)pdTICKS_TO_MS((TickType_t)left);
}

static esp_err_t at_ota_check_deadline(void)
{
    if (at_ota_timeout_remain_ms() > 0) {
        return ESP_OK;
    }
    ESP_AT_LOGE(TAG, "ota timeout");
    return ESP_ERR_TIMEOUT;
}

/* Shorter of the 10-second no-data limit and the time left in the upgrade. */
static int at_ota_io_timeout_ms(void)
{
    int remain = at_ota_timeout_remain_ms();
    if (remain <= 0) {
        return 0;
    }
    if (remain > AT_OTA_HTTP_TIMEOUT_MS) {
        return AT_OTA_HTTP_TIMEOUT_MS;
    }
    return remain;
}

static esp_err_t at_ota_arm_io_timeout(esp_http_client_handle_t client)
{
    int io_ms = at_ota_io_timeout_ms();
    if (io_ms <= 0) {
        ESP_AT_LOGE(TAG, "ota timeout");
        return ESP_ERR_TIMEOUT;
    }
    if (client != NULL && esp_http_client_set_timeout_ms(client, io_ms) != ESP_OK) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static bool at_ota_partition_is_firmware(const char *name)
{
    return strcmp(name, "ota") == 0;
}

static esp_err_t at_ota_partition_verify(const char *name, uint8_t *data, int len)
{
    int partition_num = sizeof(s_at_partition_sig) / sizeof(s_at_partition_sig[0]);

    for (int i = 0; i < partition_num; ++i) {
        if (strcmp(name, s_at_partition_sig[i].name) != 0) {
            continue;
        }
        if ((int)s_at_partition_sig[i].len > len) {
            return ESP_FAIL;
        }
        for (int loop = 0; loop < (int)s_at_partition_sig[i].len; ++loop) {
            int offset = (int)s_at_partition_sig[i].offset + loop;
            if (s_at_partition_sig[i].magic[loop] != data[offset]) {
                ESP_AT_LOGE(TAG, "%s partition has an invalid magic byte (index:%d expected 0x%02x, saw 0x%02x)",
                            name, loop, s_at_partition_sig[i].magic[loop], data[offset]);
                return ESP_FAIL;
            }
        }
        return ESP_OK;
    }

    return ESP_FAIL;
}

/* Bytes from the start of the image required before the magic can be checked. */
static int at_ota_magic_need(const char *name)
{
    int partition_num = sizeof(s_at_partition_sig) / sizeof(s_at_partition_sig[0]);

    for (int i = 0; i < partition_num; ++i) {
        if (strcmp(name, s_at_partition_sig[i].name) == 0) {
            return (int)(s_at_partition_sig[i].offset + s_at_partition_sig[i].len);
        }
    }
    return -1;
}

static esp_err_t at_ota_select_server(at_ota_mode_t mode, at_ota_server_t *server)
{
    at_ota_mode_t token_mode = mode;

    if (mode == AT_OTA_OVER_HTTP) {
        server->host = CONFIG_AT_OTA_SERVER_IP;
        server->port = CONFIG_AT_OTA_SERVER_PORT;
        server->transport = HTTP_TRANSPORT_OVER_TCP;
    }
#ifdef CONFIG_AT_OTA_SSL_SUPPORT
    else if (mode == AT_OTA_OVER_HTTPS || mode == AT_OTA_OVER_HTTPS_VERIFY) {
        server->host = CONFIG_AT_OTA_SSL_SERVER_IP;
        server->port = CONFIG_AT_OTA_SSL_SERVER_PORT;
        server->transport = HTTP_TRANSPORT_OVER_SSL;
        /* The token table has no entry for verified HTTPS. Use the HTTPS token. */
        if (mode == AT_OTA_OVER_HTTPS_VERIFY) {
            token_mode = AT_OTA_OVER_HTTPS;
        }
    }
#endif
    else {
        return ESP_ERR_INVALID_ARG;
    }

    server->token = esp_at_get_ota_token_by_id(esp_at_get_module_id(), token_mode);
    if (server->token == NULL) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

/* Resolve before connecting so +CIPUPDATE:1 is not emitted when DNS fails. */
static esp_err_t at_ota_resolve_server(const char *host)
{
    struct in_addr addr;
    if (inet_pton(AF_INET, host, &addr) != 1) {
        struct addrinfo hints = {
            .ai_family = AF_INET,
            .ai_socktype = SOCK_STREAM,
        };
        struct addrinfo *result = NULL;
        if (getaddrinfo(host, NULL, &hints, &result) != 0 || result == NULL) {
            ESP_AT_LOGE(TAG, "getaddrinfo failed");
            if (result != NULL) {
                freeaddrinfo(result);
            }
            return ESP_FAIL;
        }
        freeaddrinfo(result);
    }

    at_ota_report(AT_OTA_STATE_FOUND_SERVER);
    return ESP_OK;
}

static esp_err_t at_ota_set_auth_header(esp_http_client_handle_t client, const char *token)
{
    if (client == NULL || token == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    char *auth = NULL;
    if (asprintf(&auth, "token %s", token) < 0) {
        return ESP_ERR_NO_MEM;
    }

    esp_err_t err = esp_http_client_set_header(client, "Authorization", auth);
    free(auth);
    return err;
}

static esp_http_client_handle_t at_ota_http_client_init(const esp_http_client_config_t *config, const char *token)
{
    esp_http_client_handle_t client = esp_http_client_init(config);
    if (client == NULL) {
        return NULL;
    }
    if (at_ota_set_auth_header(client, token) != ESP_OK) {
        esp_http_client_cleanup(client);
        return NULL;
    }
    return client;
}

/* +CIPUPDATE:2 is emitted from the connect event, before the response body is read. */
static esp_err_t at_ota_http_event(esp_http_client_event_t *evt)
{
    if (evt->event_id == HTTP_EVENT_ERROR) {
        esp_at_http_handle_error_event(evt);
        return ESP_OK;
    }
    if (evt->event_id == HTTP_EVENT_ON_CONNECTED && s_ota_report_connected) {
        s_ota_report_connected = false;
        at_ota_report(AT_OTA_STATE_CONNECTED_TO_SERVER);
    }
    return ESP_OK;
}

static esp_err_t at_ota_http_open(const at_ota_server_t *server, const char *path, const char *query,
                                  bool report_connected, esp_http_client_handle_t *out_client)
{
    *out_client = NULL;

    int io_ms = at_ota_io_timeout_ms();
    if (io_ms <= 0) {
        ESP_AT_LOGE(TAG, "ota timeout");
        return ESP_ERR_TIMEOUT;
    }

    s_ota_report_connected = report_connected;
    esp_http_client_config_t config = {
        .host = server->host,
        .port = server->port,
        .path = path,
        .query = query,
        .method = HTTP_METHOD_GET,
        .transport_type = server->transport,
        .event_handler = at_ota_http_event,
        .timeout_ms = io_ms,
        .cert_pem = server->ca_pem,
        .cert_len = 0,
        .buffer_size = AT_OTA_RX_BUF_SIZE,
        .addr_type = HTTP_ADDR_TYPE_INET,
        .disable_auto_redirect = true,
    };

    esp_http_client_handle_t client = at_ota_http_client_init(&config, server->token);
    if (client == NULL) {
        return ESP_FAIL;
    }

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_AT_LOGE(TAG, "connect to ota server failed");
        esp_http_client_cleanup(client);
        return err;
    }

    if (at_ota_arm_io_timeout(client) != ESP_OK) {
        esp_http_client_cleanup(client);
        return ESP_ERR_TIMEOUT;
    }

    int64_t fetched = esp_http_client_fetch_headers(client);
    if (fetched < 0) {
        ESP_AT_LOGE(TAG, "recv data failed");
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    if (esp_http_client_get_status_code(client) >= HttpStatus_BadRequest) {
        ESP_AT_LOGE(TAG, "http status %d", esp_http_client_get_status_code(client));
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    *out_client = client;
    return ESP_OK;
}

static int at_ota_http_read(esp_http_client_handle_t client, char *buf, int len)
{
    if (at_ota_arm_io_timeout(client) != ESP_OK) {
        return -ESP_ERR_HTTP_EAGAIN;
    }
    int n = esp_http_client_read(client, buf, len);
    if (n == -ESP_ERR_HTTP_EAGAIN) {
        ESP_AT_LOGE(TAG, "recv data failed");
        return n;
    }
    /* Some bytes, then 10 seconds with no data: the client returns the bytes instead of a timeout. */
    if (n > 0 && n < len && !esp_http_client_is_complete_data_received(client)) {
        ESP_AT_LOGE(TAG, "recv data failed");
        return -ESP_ERR_HTTP_EAGAIN;
    }
    return n;
}

static esp_err_t at_ota_read_version(esp_http_client_handle_t client, char *version, size_t version_size)
{
    const int mark_len = (int)strlen(AT_OTA_VERSION_MARK);
    int matched = 0;
    int vlen = 0;
    bool in_value = false;
    bool pending_quote = false;
    esp_err_t err = ESP_FAIL;

    char *buf = malloc(AT_OTA_RX_BUF_SIZE);
    if (buf == NULL) {
        return ESP_ERR_NO_MEM;
    }

    while (1) {
        int n = at_ota_http_read(client, buf, AT_OTA_RX_BUF_SIZE);
        if (n == -ESP_ERR_HTTP_EAGAIN) {
            err = ESP_ERR_TIMEOUT;
            break;
        }
        if (n < 0) {
            ESP_AT_LOGE(TAG, "recv data failed");
            err = ESP_FAIL;
            break;
        }
        if (n == 0) {
            ESP_AT_LOGE(TAG, "%s", in_value ? "rom_version tail error" : "rom_version error");
            err = ESP_FAIL;
            break;
        }

        for (int i = 0; i < n; ++i) {
            char c = buf[i];
            if (!in_value) {
                if (c == AT_OTA_VERSION_MARK[matched]) {
                    matched++;
                    if (matched == mark_len) {
                        in_value = true;
                        vlen = 0;
                    }
                } else {
                    matched = (c == AT_OTA_VERSION_MARK[0]) ? 1 : 0;
                }
                continue;
            }

            if (pending_quote) {
                pending_quote = false;
                if (c == ',') {
                    version[vlen] = '\0';
                    err = ESP_OK;
                    goto done;
                }
                if (vlen >= AT_OTA_VERSION_LEN_MAX || (size_t)vlen + 1 >= version_size) {
                    ESP_AT_LOGE(TAG, "rom_version tail error");
                    err = ESP_FAIL;
                    goto done;
                }
                version[vlen++] = '"';
            }

            if (c == '"') {
                pending_quote = true;
                continue;
            }
            if (vlen >= AT_OTA_VERSION_LEN_MAX || (size_t)vlen + 1 >= version_size) {
                ESP_AT_LOGE(TAG, "rom_version tail error");
                err = ESP_FAIL;
                goto done;
            }
            version[vlen++] = c;
        }
    }

done:
    free(buf);
    return err;
}

static esp_err_t at_ota_query_version(const at_ota_server_t *server, char *version, size_t version_size)
{
    esp_http_client_handle_t client = NULL;
    esp_err_t err = at_ota_http_open(server, AT_OTA_ROM_PATH, AT_OTA_VERSION_QUERY, true, &client);
    if (err != ESP_OK) {
        return err;
    }

    err = at_ota_read_version(client, version, version_size);
    esp_http_client_cleanup(client);
    if (err != ESP_OK) {
        return err;
    }

    at_ota_report(AT_OTA_STATE_GOT_VERSION);
    return ESP_OK;
}

static esp_err_t at_ota_prepare_target(const char *partition_name, at_ota_target_t *target)
{
    memset(target, 0, sizeof(*target));

    if (at_ota_partition_is_firmware(partition_name)) {
        target->type = AT_UPGRADE_SYSTEM_FIRMWARE;
        if (esp_at_ota_begin(&target->ota) != ESP_OK) {
            target->ota = NULL;
            return ESP_FAIL;
        }
        ESP_AT_LOGI(TAG, "ready to upgrade system firmware...");
        return ESP_OK;
    }

    target->type = AT_UPGRADE_CUSTOM_PARTITION;
    target->partition = esp_at_custom_partition_find(0x0, 0x0, partition_name);
    if (target->partition == NULL) {
        ESP_AT_LOGE(TAG, "no custom partition: %s", partition_name);
        return ESP_FAIL;
    }

    ESP_AT_LOGI(TAG, "ready to upgrade partition: \"%s\" type:0x%x subtype:0x%x addr:0x%x size:0x%x encrypt:%d",
                target->partition->label, target->partition->type, target->partition->subtype,
                target->partition->address, target->partition->size, target->partition->encrypted);
    if (esp_partition_erase_range(target->partition, 0, target->partition->size) != ESP_OK) {
        ESP_AT_LOGE(TAG, "esp_partition_erase_range failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t at_ota_target_capacity(const at_ota_target_t *target, uint32_t *size)
{
    if (target->type == AT_UPGRADE_SYSTEM_FIRMWARE) {
        const esp_partition_t *partition = esp_at_ota_get_update_partition();
        if (partition == NULL) {
            ESP_AT_LOGE(TAG, "no ota update partition");
            return ESP_FAIL;
        }
        *size = partition->size;
        return ESP_OK;
    }

    *size = target->partition->size;
    return ESP_OK;
}

static esp_err_t at_ota_write_image(const at_ota_target_t *target, int offset, const uint8_t *data, int len)
{
    if (len <= 0) {
        return ESP_OK;
    }
    if (target->type == AT_UPGRADE_SYSTEM_FIRMWARE) {
        return esp_at_ota_write(target->ota, data, (size_t)len);
    }
    if (esp_partition_write(target->partition, (size_t)offset, data, (size_t)len) != ESP_OK) {
        ESP_AT_LOGE(TAG, "esp_partition_write failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t at_ota_download(const at_ota_server_t *server, const char *version,
                                 const char *partition_name, at_ota_target_t *target,
                                 bool report_version)
{
    char query[AT_OTA_QUERY_BUF_SIZE];
    int query_len = snprintf(query, sizeof(query), "action=download_rom&version=%s&filename=%s.bin",
                             version, partition_name);
    if (query_len < 0 || query_len >= (int)sizeof(query)) {
        return ESP_FAIL;
    }

    esp_http_client_handle_t client = NULL;
    esp_err_t err = at_ota_http_open(server, AT_OTA_ROM_PATH, query, report_version, &client);
    if (err != ESP_OK) {
        return err;
    }
    /* Explicit version has no version-list request, so :2 and :3 come from this connection. */
    if (report_version) {
        at_ota_report(AT_OTA_STATE_GOT_VERSION);
    }

    int64_t content_len = esp_http_client_get_content_length(client);
    if (content_len < 0) {
        ESP_AT_LOGE(TAG, "content length not found");
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }
    if (content_len == 0 || content_len > INT32_MAX) {
        ESP_AT_LOGE(TAG, "invalid content length: %lld", (long long)content_len);
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    uint32_t partition_size = 0;
    if (at_ota_target_capacity(target, &partition_size) != ESP_OK) {
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }
    if ((uint64_t)content_len > partition_size) {
        ESP_AT_LOGE(TAG, "ota bin oversize, %d > %u", (int)content_len, (unsigned)partition_size);
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    int total_len = (int)content_len;
    ESP_AT_LOGI(TAG, "total_len=%d", total_len);

    int magic_need = at_ota_magic_need(partition_name);
    if (magic_need <= 0 || magic_need > AT_OTA_RX_BUF_SIZE) {
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    uint8_t *buf = malloc(AT_OTA_RX_BUF_SIZE);
    if (buf == NULL) {
        esp_http_client_cleanup(client);
        return ESP_ERR_NO_MEM;
    }

    int filled = 0;
    int received = 0;
    bool verified = false;
    err = ESP_OK;

    while (received < total_len) {
        int offset = verified ? 0 : filled;
        int space = AT_OTA_RX_BUF_SIZE - offset;
        int n = at_ota_http_read(client, (char *)buf + offset, space);
        if (n == -ESP_ERR_HTTP_EAGAIN) {
            err = ESP_ERR_TIMEOUT;
            break;
        }
        if (n < 0) {
            ESP_AT_LOGE(TAG, "recv data failed");
            err = ESP_FAIL;
            break;
        }
        if (n == 0) {
            err = ESP_FAIL;
            break;
        }

        int chunk = n;
        if (!verified) {
            filled += n;
            if (filled < magic_need) {
                continue;
            }
            if (at_ota_partition_verify(partition_name, buf, filled) != ESP_OK) {
                err = ESP_FAIL;
                break;
            }
            verified = true;
            chunk = filled;
        }

        int remain = total_len - received;
        if (chunk > remain) {
            chunk = remain;
        }
        if (at_ota_write_image(target, received, buf, chunk) != ESP_OK) {
            err = ESP_FAIL;
            break;
        }
        received += chunk;
        ESP_AT_LOGI(TAG, "Downloading... %d%% (%d/%d)",
                    (int)(((int64_t)received * 100) / total_len),
                    received, total_len);
    }

    free(buf);
    esp_http_client_cleanup(client);

    if (err == ESP_OK && (!verified || received != total_len)) {
        err = ESP_FAIL;
    }
    return err;
}

static bool at_ota_upgrade(at_ota_mode_t ota_mode, const char *version, const char *partition_name)
{
    at_ota_server_t server = {0};
    at_ota_target_t target = {0};
    char version_buf[AT_OTA_VERSION_LEN_MAX + 1] = {0};
    const char *ver = version;
    bool ok = false;

    if (partition_name == NULL || partition_name[0] == '\0') {
        return false;
    }

    ESP_AT_LOGI(TAG, "ota_mode: %d", ota_mode);
    if (at_ota_select_server(ota_mode, &server) != ESP_OK) {
        return false;
    }

    if (ota_mode == AT_OTA_OVER_HTTPS_VERIFY) {
        /* AT+CIUPDATE has no CA index. Use the same default set as AT+USEROTA. */
        if (esp_at_ota_load_ca(0, &server.ca_pem) != ESP_OK || esp_at_ota_check_time() != ESP_OK) {
            free(server.ca_pem);
            return false;
        }
    }

    at_ota_timeout_start();
    if (at_ota_resolve_server(server.host) != ESP_OK) {
        goto out;
    }

    const bool version_given = (ver != NULL);
    if (!version_given) {
        if (at_ota_query_version(&server, version_buf, sizeof(version_buf)) != ESP_OK) {
            goto out;
        }
        ver = version_buf;
    }
    ESP_AT_LOGI(TAG, "version: %s", ver);

    if (at_ota_prepare_target(partition_name, &target) != ESP_OK) {
        goto out;
    }
    if (at_ota_download(&server, ver, partition_name, &target, version_given) != ESP_OK) {
        goto out;
    }
    if (at_ota_check_deadline() != ESP_OK) {
        goto out;
    }

    if (target.type == AT_UPGRADE_SYSTEM_FIRMWARE) {
        if (esp_at_ota_end(target.ota) != ESP_OK) {
            target.ota = NULL;
            goto out;
        }
        target.ota = NULL;
    }

    at_ota_report(AT_OTA_STATE_DONE);
    ok = true;

out:
    if (target.ota != NULL) {
        esp_at_ota_abort(target.ota);
    }
    free(server.ca_pem);
    return ok;
}

static void at_ota_restart(void)
{
    esp_at_write_result(ESP_AT_RESULT_CODE_OK);
    esp_at_port_wait_write_complete(ESP_AT_PORT_TX_WAIT_MS_MAX);
    esp_at_restart();
    for (;;) {
    }
}

static uint8_t at_exe_cmd_ciupdate(uint8_t *cmd_name)
{
    (void)cmd_name;

    if (s_ota_started) {
        ESP_AT_LOGW(TAG, "ALREADY IN OTA");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (at_ota_upgrade(AT_OTA_OVER_HTTP, NULL, "ota")) {
        at_ota_restart();
    }

    at_ota_set_state(AT_OTA_STATE_FAILED);
    return ESP_AT_RESULT_CODE_ERROR;
}

static void at_ota_nonblocking_task(void *parameter)
{
    at_ota_param_t *param = parameter;
    /* calloc leaves this empty when the command omitted the version. */
    const char *version = param->version[0] != '\0' ? param->version : NULL;

    if (at_ota_upgrade((at_ota_mode_t)param->ota_mode, version, param->partition_name)) {
        esp_at_port_wait_write_complete(ESP_AT_PORT_TX_WAIT_MS_MAX);
    } else {
        at_ota_report(AT_OTA_STATE_FAILED);
    }

    free(param);
    s_ota_started = false;
    vTaskDelete(NULL);
}

static uint8_t at_setup_cmd_ciupdate(uint8_t para_num)
{
    at_ota_param_t *param = NULL;
    uint8_t *version = NULL;
    uint8_t *partition_name = NULL;
    int32_t cnt = 0;
    int32_t ota_mode = 0;
    int32_t nonblocking = 0;
    int32_t version_len = 0;

    if (esp_at_get_para_as_digit(cnt++, &ota_mode) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((ota_mode < 0) || (ota_mode >= AT_OTA_OVER_MAX)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt < para_num) {
        if (esp_at_get_para_as_str(cnt++, &version) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (version != NULL) {
            version_len = strlen((const char *)version);
            if (version_len == 0) {
                version = NULL;
            } else if (version_len > AT_OTA_VERSION_LEN_MAX) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }
    }

    if (cnt < para_num) {
        if (esp_at_get_para_as_str(cnt++, &partition_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if ((partition_name == NULL) || (strlen((const char *)partition_name) == 0)) {
        partition_name = (uint8_t *)"ota";
    } else if (strlen((const char *)partition_name) > AT_OTA_PARTITION_NAME_LEN_MAX) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt < para_num) {
        if (esp_at_get_para_as_digit(cnt++, &nonblocking) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }
    if ((nonblocking < 0) || (nonblocking >= AT_OTA_NONBLOCKING_MODE_MAX)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (s_ota_started) {
        ESP_AT_LOGW(TAG, "ALREADY IN OTA");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (nonblocking == 1) {
        param = calloc(1, sizeof(at_ota_param_t));
        if (param == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        param->ota_mode = ota_mode;
        if (version != NULL) {
            memcpy(param->version, version, version_len);
        }
        memcpy(param->partition_name, partition_name, strlen((const char *)partition_name));

        s_ota_started = true;
        if (xTaskCreate(at_ota_nonblocking_task, "nb-ota", AT_OTA_NB_TASK_STACK_SIZE, param, 1, NULL) != pdPASS) {
            s_ota_started = false;
            free(param);
            return ESP_AT_RESULT_CODE_ERROR;
        }
        return ESP_AT_RESULT_CODE_OK;
    }

    if (at_ota_upgrade(ota_mode, (const char *)version, (const char *)partition_name)) {
        if (at_ota_partition_is_firmware((const char *)partition_name)) {
            at_ota_restart();
        }
        return ESP_AT_RESULT_CODE_OK;
    }

    at_ota_set_state(AT_OTA_STATE_FAILED);
    return ESP_AT_RESULT_CODE_ERROR;
}

static uint8_t at_query_cmd_ciupdate(uint8_t *cmd_name)
{
    char buffer[32] = {0};
    snprintf(buffer, sizeof(buffer), "%s:%d\r\n", cmd_name, (int)s_ota_status);
    esp_at_port_write_data((uint8_t *)buffer, strlen(buffer));
    return ESP_AT_RESULT_CODE_OK;
}

typedef struct {
    uint8_t *data;
    int offset;
    bool overflow;
} at_ota_fwver_buf_t;

static esp_err_t at_ota_fwver_event(esp_http_client_event_t *evt)
{
    at_ota_fwver_buf_t *body = evt->user_data;
    int remain;

    if (evt->event_id == HTTP_EVENT_ERROR) {
        esp_at_http_handle_error_event(evt);
        return ESP_OK;
    }
    if (body == NULL) {
        return ESP_FAIL;
    }
    if (evt->event_id != HTTP_EVENT_ON_DATA) {
        return ESP_OK;
    }

    /* Keep one byte for the NUL terminator used by the version parser.
     * esp_http_client_perform() ignores this callback's return value, so the
     * overflow flag is what makes a truncated body fail the command.
     */
    if (body->overflow) {
        return ESP_FAIL;
    }
    if (body->data == NULL || evt->data == NULL || evt->data_len < 0 || body->offset < 0) {
        body->overflow = true;
        return ESP_FAIL;
    }
    if (evt->data_len == 0) {
        return ESP_OK;
    }

    remain = AT_OTA_FWVER_BODY_MAX - 1 - body->offset;
    if (body->offset >= AT_OTA_FWVER_BODY_MAX - 1 || evt->data_len > remain) {
        body->overflow = true;
        return ESP_FAIL;
    }

    memcpy(body->data + body->offset, evt->data, evt->data_len);
    body->offset += evt->data_len;
    return ESP_OK;
}

static uint8_t at_query_cmd_cipfwver(uint8_t *cmd_name)
{
    (void)cmd_name;
    uint8_t ret = ESP_AT_RESULT_CODE_ERROR;
    at_ota_fwver_buf_t body = {0};

    esp_http_client_config_t config = {
#ifdef CONFIG_AT_OTA_SSL_SUPPORT
        .host = CONFIG_AT_OTA_SSL_SERVER_IP,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
#else
        .host = CONFIG_AT_OTA_SERVER_IP,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
#endif
        .path = "/v1/device/rom",
        .event_handler = at_ota_fwver_event,
        .timeout_ms = AT_OTA_HTTP_TIMEOUT_MS,
        .user_data = &body,
    };

#ifdef CONFIG_AT_OTA_SSL_SUPPORT
    const char *auth_token = esp_at_get_ota_token_by_id(esp_at_get_module_id(), AT_OTA_OVER_HTTPS);
#else
    const char *auth_token = esp_at_get_ota_token_by_id(esp_at_get_module_id(), AT_OTA_OVER_HTTP);
#endif
    esp_http_client_handle_t client = at_ota_http_client_init(&config, auth_token);
    if (client == NULL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    body.data = calloc(1, AT_OTA_FWVER_BODY_MAX);
    if (body.data == NULL) {
        esp_http_client_cleanup(client);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_http_client_perform(client) != ESP_OK || body.overflow) {
        if (body.overflow) {
            ESP_AT_LOGE(TAG, "fwver response too long");
        }
        goto over;
    }
    if (esp_http_client_get_status_code(client) >= 400 || body.data == NULL) {
        goto over;
    }

    char *pstr = (char *)body.data;
    char buffer[ESP_AT_BUF_ON_STACK_SIZE];
    int found = 0;
    while ((uint8_t *)pstr < body.data + body.offset) {
        pstr = strstr(pstr, "\"version\": ");
        if (pstr == NULL) {
            break;
        }

        char *ver_head = pstr + strlen("\"version\": ");
        if (ver_head >= (char *)(body.data + body.offset)) {
            break;
        }
        char *ver_tail = strstr(ver_head, "\", ");
        if (ver_tail == NULL) {
            break;
        }

        int precision = (int)(ver_tail - ver_head) + 1;
        if (precision <= 0 || precision >= ESP_AT_BUF_ON_STACK_SIZE) {
            break;
        }
        snprintf(buffer, sizeof(buffer), "%s:%.*s\r\n", esp_at_get_current_cmd_name(), precision, ver_head);
        esp_at_port_write_data((uint8_t *)buffer, strlen(buffer));
        found++;
        pstr = ver_tail;
    }
    if (found > 0) {
        ret = ESP_AT_RESULT_CODE_OK;
    }

over:
    free(body.data);
    esp_http_client_cleanup(client);
    return ret;
}

static const esp_at_cmd_t at_upgrade_cmd[] = {
    {"+CIUPDATE", NULL, at_query_cmd_ciupdate, at_setup_cmd_ciupdate, at_exe_cmd_ciupdate},
    {"+CIPFWVER", NULL, at_query_cmd_cipfwver, NULL, NULL},
};

bool esp_at_ota_cmd_register(void)
{
    return esp_at_custom_cmd_array_register(at_upgrade_cmd, sizeof(at_upgrade_cmd) / sizeof(at_upgrade_cmd[0]));
}

ESP_AT_CMD_SET_FIRST_INIT_FN(esp_at_ota_cmd_register, 22);

#endif
