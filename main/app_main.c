/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_err.h"

#include "driver/adc.h"
#include "esp_mqtt_client.h"
#include "nvs.h"

#include "esp_at.h"
#include "esp_at_init.h"

static TimerHandle_t adc_timer = NULL;
static esp_mqtt_client_handle_t mqtt_client = NULL;


// 初始化MQTT客户端
static void mqtt_init() {
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = 192.168.1.100,
        .port = 1883,
        .client_id = "esp32c3-adc",
        .username = "zhanghuohuo",
        .password = "zhang1432"
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(mqtt_client);
}

// ADC定时读取并发送
static void adc_publish_task(TimerHandle_t xTimer) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    int adc_val = adc1_get_raw(ADC1_CHANNEL_0);
    
    char payload[16];
    snprintf(payload, sizeof(payload), "%d", adc_val);
    esp_mqtt_client_publish(mqtt_client, "sensor/adc", payload, 0, 1, 0);
}


void app_main(void)
{
        // 初始化NVS
    nvs_flash_init();
    // 初始化MQTT（自动连接）
    mqtt_init();
    // 创建ADC定时任务（立即启动，每5秒5000执行一次）
    adc_timer = xTimerCreate("ADC_Publish", 
                            pdMS_TO_TICKS(50), 
                            pdTRUE, 
                            NULL, 
                            adc_publish_task);
    xTimerStart(adc_timer, 0);



    // 其他AT命令初始化...
    esp_at_main_preprocess();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_at_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_at_init();
}
