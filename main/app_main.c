/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_err.h"
#include <stdio.h>  // 补充在文件头部
//#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"  // 单次采样模式
#include "esp_mqtt_client.h"
#include "nvs.h"

#include "esp_at.h"
#include "esp_at_init.h"

static TimerHandle_t adc_timer = NULL;
static esp_mqtt_client_handle_t mqtt_client = NULL;
// 全局定义ADC句柄
static adc_oneshot_unit_handle_t adc_handle = NULL;

// 初始化MQTT客户端
static void mqtt_init() {
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://192.168.1.100",
        .port = 1883,
        .client_id = "esp32c3-adc",
        .username = "zhanghuohuo",
        .password = "zhang1432"
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(mqtt_client);
}

// 全局定义ADC句柄
static adc_oneshot_unit_handle_t adc_handle = NULL;

// ADC初始化函数（在任务外初始化）
static void adc_init() {
    adc_oneshot_unit_init_cfg_t adc_config = {
        .unit_id = ADC_UNIT_1,  // 根据硬件选择ADC_UNIT_1或ADC_UNIT_2
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_config, &adc_handle));

    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH_12,  // 12位精度
        .atten = ADC_ATTEN_DB_11,      // 衰减11dB
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_0, &chan_config));
}

// ADC读取任务
static void adc_publish_task(TimerHandle_t xTimer) {
    int adc_val = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_0, &adc_val));
    
    char payload[16];
    snprintf(payload, sizeof(payload), "%d", adc_val);
    esp_mqtt_client_publish(mqtt_client, "sensor/adc", payload, 0, 1, 0);
}

void app_main(void)
{

    // // 初始化MQTT（自动连接）
    mqtt_init();
    adc_init();
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
