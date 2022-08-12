/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2022-2025 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#ifdef CONFIG_BOOTLOADER_COMPRESSED_ENABLED
#include "esp_rom_crc.h"
#include "esp_rom_md5.h"
#include "esp_http_client.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "bootloader_custom_ota.h"
#include "at_compress_ota.h"

#define AT_COMPRESSED_IMAGE_MAGIC_NUMBER        BOOTLOADER_CUSTOM_OTA_HEADER_MAGIC
#define AT_COMPRESS_OTA_PARTITION_SUBTYPE       BOOTLOADER_CUSTOM_OTA_PARTITION_SUBTYPE
#define AT_HEAP_BUFFER_SIZE                     4096

static const char *TAG = "compress-ota";

esp_err_t at_compress_ota_begin(at_compress_ota_handle_t *handle)
{
    if (!handle) {
        return ESP_FAIL;
    }

    const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, AT_COMPRESS_OTA_PARTITION_SUBTYPE, NULL);
    if (partition) {
        ESP_LOGD(TAG, "Found partition, offset:0x%x size:0x%x", partition->address, partition->size);
    } else {
        ESP_LOGE(TAG, "No partition, type:%d subtype:%d", ESP_PARTITION_TYPE_DATA, AT_COMPRESS_OTA_PARTITION_SUBTYPE);
        return ESP_FAIL;
    }

    esp_err_t ret = esp_partition_erase_range(partition, 0, partition->size);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to erase partition: %s", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    handle->wrote_size = 0;
    handle->compressed_img_size = 0;
    handle->image_header_checked = false;
    handle->partition = partition;

    return ESP_OK;
}

static esp_err_t at_compress_image_header_check(at_compress_ota_handle_t *handle, bootloader_custom_ota_header_t *compressed_img_header)
{
    if (!handle || !compressed_img_header) {
        return ESP_FAIL;
    }

    uint32_t head_len = sizeof(compressed_img_header->header_v2);

    // check the magic number of compressed image
    if (memcmp(compressed_img_header, AT_COMPRESSED_IMAGE_MAGIC_NUMBER, strlen(AT_COMPRESSED_IMAGE_MAGIC_NUMBER))) {
        ESP_LOGE(TAG, "Compressed image has invalid magic bytes (expected:%s, saw:%.*s)",
            AT_COMPRESSED_IMAGE_MAGIC_NUMBER, strlen(AT_COMPRESSED_IMAGE_MAGIC_NUMBER), compressed_img_header);
        return ESP_FAIL;
    }

    // check the header version compressed image
    if (compressed_img_header->header.version != 2) {
        ESP_LOGE(TAG, "Compressed image has invalid header version (expected:2, saw:%d)", compressed_img_header->header.version);
        return ESP_FAIL;
    }

    // check the size of compressed image, make sure the partition can hold on the compressed image
    if (compressed_img_header->header.length + head_len <= handle->partition->size) {
        handle->compressed_img_size = compressed_img_header->header.length + head_len;
        handle->image_header_checked = true;
        ESP_LOGD(TAG, "Compressed image header check succeeded, image size: %d", handle->compressed_img_size);
    } else {
        ESP_LOGE(TAG, "Compressed image overlength, image size:%d > psize:%d (0x%x)",
            compressed_img_header->header.length + head_len, handle->partition->size, handle->partition->size);
        return ESP_FAIL;
    }

    // check the crc32 of compressed image header
    uint32_t header_crc = esp_rom_crc32_le(0, (const uint8_t *)compressed_img_header, offsetof(bootloader_custom_ota_header_t, header_v2.crc32));
    if (header_crc != compressed_img_header->header_v2.crc32) {
        ESP_LOGE(TAG, "Unmatched compressed image header crc (expected:0x%x saw:0x%x)", compressed_img_header->header_v2.crc32, header_crc);
        return ESP_FAIL;
    }

    return ESP_OK;
}

static esp_err_t at_compress_image_body_check(at_compress_ota_handle_t *handle, bootloader_custom_ota_header_t *compressed_img_header)
{
    esp_err_t ret = ESP_FAIL;

    uint8_t *data = (uint8_t *)malloc(AT_HEAP_BUFFER_SIZE);
    if (!data) {
        ESP_LOGE(TAG, "malloc failed");
        return ESP_FAIL;
    }

    uint32_t had_read_len = 0;
    uint32_t img_body_len = compressed_img_header->header.length;
    uint32_t head_len = sizeof(compressed_img_header->header_v2);

    md5_context_t md5_context;
    esp_rom_md5_init(&md5_context);
    do {
        int to_read_len = (img_body_len - had_read_len > AT_HEAP_BUFFER_SIZE) ? AT_HEAP_BUFFER_SIZE : (img_body_len - had_read_len);
        ret = esp_partition_read(handle->partition, head_len + had_read_len, data, to_read_len);
        if (ret != ESP_OK) {
            break;
        }
        esp_rom_md5_update(&md5_context, data, to_read_len);
        had_read_len += to_read_len;
    } while (ret == ESP_OK && had_read_len < img_body_len);

    uint8_t digest[16] = {0};
    esp_rom_md5_final(digest, &md5_context);

    if (!memcmp(compressed_img_header->header.md5, digest, sizeof(digest))) {
        ESP_LOGD(TAG, "Compressed image MD5 check succeeded");
        ret = ESP_OK;
    } else {
        ESP_LOGE(TAG, "Compressed image MD5 check failed");
        ret = ESP_FAIL;
    }

    free(data);
    return ret;
}

esp_err_t at_compress_ota_write(at_compress_ota_handle_t *handle, const void *data, int size)
{
    if (!handle || !data || size < 0) {
        ESP_LOGE(TAG, "Invalid input parameters, handle:%p, data:%p, size:%d", handle, data, size);
        return ESP_FAIL;
    }

    if (size < 0) {
        return ESP_OK;
    }

    if (!handle->image_header_checked) {
        uint32_t head_len = sizeof(bootloader_custom_ota_header_v2_t);
        if (size >= head_len) {
            if (at_compress_image_header_check(handle, (bootloader_custom_ota_header_t *)data) != ESP_OK) {
                return ESP_FAIL;
            }
        } else {
            ESP_LOGE(TAG, "Recv insufficient header: %d", size);
            return ESP_FAIL;
        }
    }

    esp_err_t ret = esp_partition_write(handle->partition, handle->wrote_size, data, size);
    if (ret == ESP_OK) {
        handle->wrote_size += size;
    } else {
        ESP_LOGE(TAG, "Failed to write compressed image: %s", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t at_compress_ota_end(at_compress_ota_handle_t *handle)
{
    if (!handle) {
        return ESP_FAIL;
    }

    if (handle->wrote_size < handle->compressed_img_size) {
        ESP_LOGE(TAG, "Unmatched compressed image size (expected:%d, saw:%d)", handle->compressed_img_size, handle->wrote_size);
        return ESP_FAIL;
    } else if (handle->wrote_size > handle->compressed_img_size) {
        ESP_LOGW(TAG, "Unmatched compressed image size (expected:%d, saw:%d)", handle->compressed_img_size, handle->wrote_size);
    }

    bootloader_custom_ota_header_t compressed_img_header;
    if (esp_partition_read(handle->partition, 0, &compressed_img_header, sizeof(compressed_img_header)) != ESP_OK) {
        return ESP_FAIL;
    }

    // check compressed image header
    if (at_compress_image_header_check(handle, &compressed_img_header) != ESP_OK) {
        return ESP_FAIL;
    }

    // check compressed image body
    if (at_compress_image_body_check(handle, &compressed_img_header) != ESP_OK) {
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Compressed image integrity verification succeeded!");

    return ESP_OK;
}

esp_err_t at_compress_https_ota(const esp_http_client_config_t *config)
{
    if (!config) {
        return ESP_FAIL;
    }

    at_compress_ota_handle_t handle;
    if (at_compress_ota_begin(&handle) != ESP_OK) {
        return ESP_FAIL;
    }

    esp_http_client_handle_t client = esp_http_client_init(config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialise HTTP connection");
        return ESP_FAIL;
    }

    esp_err_t ret = esp_http_client_open(client, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(ret));
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    esp_http_client_fetch_headers(client);
    int status_code = esp_http_client_get_status_code(client);
    if (status_code >= HttpStatus_BadRequest) {
        ESP_LOGE(TAG, "HTTP client error (%d)", status_code);
        return ESP_FAIL;
    }

    uint8_t *data = (uint8_t *)malloc(AT_HEAP_BUFFER_SIZE);
    if (!data) {
        return ESP_FAIL;
    }

    int data_len = 0;
    do {
        data_len = esp_http_client_read(client, (char *)data, AT_HEAP_BUFFER_SIZE);
        if (data_len > 0) {
            ret = at_compress_ota_write(&handle, data, data_len);
            if (ret != ESP_OK) {
                break;
            }
        } else if (data_len < 0) {
            ESP_LOGE(TAG, "Connection aborted");
            break;
        }
    } while (ret == ESP_OK && data_len > 0);

    esp_http_client_cleanup(client);
    free(data);

    return (ret == ESP_OK ? at_compress_ota_end(&handle) : ret);
}

#endif
