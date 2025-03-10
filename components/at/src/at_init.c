/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"

#if defined(CONFIG_BT_ENABLED)
#include "esp_bt.h"
#endif

#if defined(CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE) && !defined(CONFIG_BOOTLOADER_COMPRESSED_ENABLED)
#include "esp_ota_ops.h"
#endif

#include "esp_at_core.h"
#include "esp_at.h"
#include "esp_at_init.h"
#include "esp_at_interface.h"

// global variables
const char *g_at_mfg_nvs_name = "mfg_nvs";

// static variables
static const char *s_ready_str = "\r\nready\r\n";
static at_mfg_params_storage_mode_t s_at_param_mode = AT_PARAMS_NONE;
static const char *TAG = "at-init";

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
esp_err_t at_wifi_init(void)
{
    esp_err_t ret;
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) {
        return ret;
    }

    return esp_wifi_start();
}

esp_err_t at_wifi_deinit(void)
{
    esp_wifi_stop();
    return esp_wifi_deinit();
}
#endif

#if CONFIG_AT_WIFI_DUMP_STATIS_DEBUG
static void at_wifi_statistics_task(void *params)
{
    while (1) {
        vTaskDelay(CONFIG_AT_WIFI_DUMP_STATIS_INTV_MS / portTICK_PERIOD_MS);
        esp_wifi_statis_dump(0xFFFFFFFF);
    }
}
#endif

esp_err_t esp_at_netif_init(void)
{
#if defined(CONFIG_AT_WIFI_COMMAND_SUPPORT) || defined(CONFIG_AT_ETHERNET_SUPPORT)
    return esp_netif_init();
#else
    return ESP_OK;
#endif
}

static void at_module_init(void)
{
#define AT_TEMP_BUFFER_SIZE     256
    int ret = 0;
    uint8_t *version = (uint8_t *)malloc(AT_TEMP_BUFFER_SIZE);
    ret = snprintf((char *)version, AT_TEMP_BUFFER_SIZE,
                   "compile time(%s):%s %s\r\n", ESP_AT_PROJECT_COMMIT_ID, __DATE__, __TIME__);

#ifdef CONFIG_APP_PROJECT_VER
#ifdef ESP_AT_FIRMWARE_FROM
    ESP_AT_LOGI(TAG, "%s (%s)", CONFIG_APP_PROJECT_VER, ESP_AT_FIRMWARE_FROM);
#else
    ESP_AT_LOGI(TAG, "%s (unknown)", CONFIG_APP_PROJECT_VER);
#endif

    ret = snprintf((char *)version + ret, AT_TEMP_BUFFER_SIZE - ret,
                   "Bin version:%s(%s)\r\n", CONFIG_APP_PROJECT_VER, esp_at_get_current_module_name());
#endif

#if CONFIG_AT_DEBUG
    char *versions = (char *)malloc(AT_TEMP_BUFFER_SIZE);
    ret = esp_at_get_core_version((char *)versions, AT_TEMP_BUFFER_SIZE);
    ret += snprintf((char *)versions + ret, AT_TEMP_BUFFER_SIZE - ret, "SDK version:%s\r\n", esp_get_idf_version());
    ret += snprintf((char *)versions + ret, AT_TEMP_BUFFER_SIZE - ret, "%s", version);
    ESP_AT_LOGI(TAG, "%.*s", ret, versions);
    free(versions);
#endif

    esp_at_module_init(version);
    free(version);
    ESP_LOGD(TAG, "at module init done");
}

#ifdef CONFIG_AT_COMMAND_TERMINATOR_SUPPORT
static void at_cmd_set_terminator(uint8_t c)
{
    uint8_t terminator[] = {c, 0};
    esp_at_custom_cmd_line_terminator_set((uint8_t *)&terminator);
}
#endif

#ifdef CONFIG_BT_ENABLED
static void at_bt_controller_mem_release(void)
{
#if defined(CONFIG_AT_BLE_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_HID_COMMAND_SUPPORT) || defined(CONFIG_AT_BLUFI_COMMAND_SUPPORT)
#if !defined(CONFIG_AT_BT_COMMAND_SUPPORT)
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
#endif
#else
#if defined(CONFIG_AT_BT_COMMAND_SUPPORT)
    esp_bt_controller_mem_release(ESP_BT_MODE_BLE);
#else
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
#endif
#endif
}
#endif

__attribute__((weak)) void esp_at_ready_before(void)
{
#ifdef CONFIG_AT_SELF_COMMAND_SUPPORT
    at_exe_cmd("AT+GMR\r\n", "OK", 1000);
    at_exe_cmd("AT+SYSRAM?\r\n", "OK", 1000);
#endif
}

static esp_err_t at_module_config_init(void)
{
    char buffer[AT_BUFFER_ON_STACK_SIZE] = {0};

    at_mfg_params_storage_mode_t mode = at_get_mfg_params_storage_mode();
    if (mode == AT_PARAMS_IN_MFG_NVS) {
        nvs_handle handle;
        if (nvs_open_from_partition(g_at_mfg_nvs_name, "factory_param", NVS_READONLY, &handle) != ESP_OK) {
            ESP_AT_LOGE(TAG, "open factory_param failed");
            return ESP_FAIL;
        }
        // read module name from manufacturing nvs
        size_t len = AT_BUFFER_ON_STACK_SIZE;
        if (esp_at_nvs_get_str(handle, "module_name", buffer, &len) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        esp_at_set_module_id_by_str(buffer);
        nvs_close(handle);
    } else if (mode == AT_PARAMS_IN_PARTITION) {
        // deprecated way
        const esp_partition_t *partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
        if (!partition) {
            ESP_AT_LOGE(TAG, "factory_param partition missed");
            return ESP_FAIL;
        }
        if (esp_partition_read(partition, 0, buffer, AT_BUFFER_ON_STACK_SIZE) != ESP_OK) {
            return ESP_FAIL;
        }
        // check magic flag, should be 0xfc 0xfc
        if (buffer[0] != 0xFC || buffer[1] != 0xFC) {
            return ESP_FAIL;
        }
        uint8_t version = buffer[2];
        if (version <= 2) {
            esp_at_set_module_id(buffer[3]);
        } else {
            const char *module_name = buffer + 56;
            esp_at_set_module_id_by_str(module_name);
        }
    } else {
        return ESP_FAIL;
    }
    ESP_AT_LOGI(TAG, "module_name: %s", esp_at_get_current_module_name());

    return ESP_OK;
}

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
static esp_err_t at_wifi_config_init(void)
{
    char buffer[AT_BUFFER_ON_STACK_SIZE] = {0};

    at_mfg_params_storage_mode_t mode = at_get_mfg_params_storage_mode();
    if (mode == AT_PARAMS_IN_MFG_NVS) {
        nvs_handle handle;
        if (nvs_open_from_partition(g_at_mfg_nvs_name, "factory_param", NVS_READONLY, &handle) != ESP_OK) {
            ESP_AT_LOGE(TAG, "open factory_param failed");
            return ESP_FAIL;

        }
        esp_wifi_set_storage(WIFI_STORAGE_RAM);

        // max tx power
        int8_t tx_power = -1;
        if (nvs_get_i8(handle, "max_tx_power", &tx_power) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        esp_err_t ret = esp_wifi_set_max_tx_power(tx_power);
        ESP_AT_LOGI(TAG, "max tx power=%d, ret=%d", tx_power, ret);

        // country code
        wifi_country_t country;
        memset(&country, 0x0, sizeof(country));
        if (nvs_get_u8(handle, "start_channel", &country.schan) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        if (nvs_get_u8(handle, "channel_num", &country.nchan) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        size_t len = AT_BUFFER_ON_STACK_SIZE;
        if (esp_at_nvs_get_str(handle, "country_code", buffer, &len) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        memcpy(country.cc, buffer, sizeof(country.cc));
        country.policy = WIFI_COUNTRY_POLICY_MANUAL;
        esp_wifi_set_country(&country);
        nvs_close(handle);
    } else if (mode == AT_PARAMS_IN_PARTITION) {
        // deprecated way
        const esp_partition_t *partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
        if (!partition) {
            ESP_AT_LOGE(TAG, "factory_param partition missed");
            return ESP_FAIL;
        }
        if (esp_partition_read(partition, 0, buffer, AT_BUFFER_ON_STACK_SIZE) != ESP_OK) {
            return ESP_FAIL;
        }
        // check magic flag, should be 0xfc 0xfc
        if (buffer[0] != 0xFC || buffer[1] != 0xFC) {
            return ESP_FAIL;
        }
        esp_wifi_set_storage(WIFI_STORAGE_RAM);

        // max tx power
        uint8_t version = buffer[2];
        if (buffer[4] != 0xFF) {
            if ((version != 1) || ((version == 1) && (buffer[4] >= 10))) {
                esp_err_t ret = esp_wifi_set_max_tx_power((int8_t)buffer[4]);
                ESP_AT_LOGI(TAG, "max tx power=%d, ret=%d", buffer[4], ret);
            }
        }

        // country code
        wifi_country_t country;
        memset(&country, 0x0, sizeof(country));
        if ((buffer[6] != 0xFF) && (buffer[7] != 0xFF) && (buffer[8] != 0xFF)) {
            if ((buffer[6] < 1) || (buffer[7] > 14) || (buffer[7] < buffer[6])) {
                ESP_AT_LOGE(TAG, "invalid country code, s:%d n:%d", buffer[6], buffer[7]);
                return ESP_FAIL;
            }
            country.schan = buffer[6];
            country.nchan = buffer[7] - buffer[6] + 1;
            memcpy(country.cc, &buffer[8], sizeof(country.cc));
            country.policy = WIFI_COUNTRY_POLICY_MANUAL;
            esp_wifi_set_country(&country);
        }
    } else {
        return ESP_FAIL;
    }

    ESP_LOGD(TAG, "at wifi init done");
    return ESP_OK;
}
#endif

at_mfg_params_storage_mode_t at_get_mfg_params_storage_mode(void)
{
    return s_at_param_mode;
}

static void at_nvs_flash_init_partition(void)
{
    const esp_partition_t *partition = esp_at_custom_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, g_at_mfg_nvs_name);
    if (partition) {
        if (nvs_flash_init_partition_ptr(partition) != ESP_OK) {
            ESP_AT_LOGE(TAG, "init partition ptr failed");
        } else {
            s_at_param_mode = AT_PARAMS_IN_MFG_NVS;
        }
    } else if (esp_at_custom_partition_find(0x40, 0xff, "factory_param")) {
        s_at_param_mode = AT_PARAMS_IN_PARTITION;
    } else {
        s_at_param_mode = AT_PARAMS_NONE;
    }

    ESP_AT_LOGI(TAG, "at param mode: %d", s_at_param_mode);
}

static void esp_at_ready(void)
{
    esp_at_port_active_write_data((uint8_t *)s_ready_str, strlen(s_ready_str));
}

static IRAM_ATTR void at_alloc_failed_cb(size_t requested_size, uint32_t caps, const char *function_name)
{
    esp_rom_printf(DRAM_STR(LOG_ANSI_COLOR_REGULAR(LOG_ANSI_COLOR_RED) "alloc failed, size:%u, caps:0x%x" LOG_ANSI_COLOR_RESET "\n"), requested_size, caps);
}

#if defined(CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE) && !defined(CONFIG_BOOTLOADER_COMPRESSED_ENABLED)
static void at_ota_mark_app_valid_cancel_rollback(void)
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state;
    if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK) {
        if (ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
            esp_ota_mark_app_valid_cancel_rollback();
        }
    }
}
#endif

void esp_at_init(void)
{
    // set log level to max
    esp_log_level_set("*", ESP_LOG_MAX);

    // register the callback function to be invoked if a memory allocation operation fails
    heap_caps_register_failed_alloc_callback(at_alloc_failed_cb);

    // initialize the manufacturing nvs partition
    at_nvs_flash_init_partition();

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    // initialize the interface for wifi station and softap
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();
    at_wifi_init();

#ifdef CONFIG_AT_WIFI_DUMP_STATIS_DEBUG
    xTaskCreate(at_wifi_statistics_task, "wifi-dbg", 2048, NULL, 1, NULL);
#endif
#endif

    // initialize the interface for esp-at and mcu communication
    at_interface_init();

    // initialize the module configuration based on the parameters in the manufacturing partition
    at_module_config_init();

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    // initialize the wifi configuration based on the parameters in the manufacturing partition
    at_wifi_config_init();
#endif

    // initialize the AT framework (init task, queue, at cmd parser, at cmd responder, etc)
    at_module_init();

    // register all the at command set
    esp_at_cmd_set_register();

#ifdef CONFIG_BT_ENABLED
    // release possible memory allocated by the bt controller
    at_bt_controller_mem_release();
#endif

#ifdef CONFIG_AT_COMMAND_TERMINATOR_SUPPORT
    // set the AT command terminator
    at_cmd_set_terminator(CONFIG_AT_COMMAND_TERMINATOR);
#endif

    // do some special things before AT is ready
    esp_at_ready_before();

#if defined(CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE) && !defined(CONFIG_BOOTLOADER_COMPRESSED_ENABLED)
    // indicate that the running app is working well for app rollback
    at_ota_mark_app_valid_cancel_rollback();
#endif

    // once the interface is started, the AT command can be received and processed
    at_interface_start();

    esp_at_ready();
    ESP_LOGD(TAG, "esp_at_init done");
}
