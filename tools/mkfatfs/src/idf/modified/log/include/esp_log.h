#pragma once

//MVA emulate log system

extern int g_debugLevel;

#define ESP_LOGE(x, ...) {if(g_debugLevel >= 0) {printf("E %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define ESP_LOGW(x, ...) {if(g_debugLevel >= 1) {printf("W %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define ESP_LOGI(x, ...) {if(g_debugLevel >= 2) {printf("I %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define ESP_LOGD(x, ...) {if(g_debugLevel >= 3) {printf("D %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define ESP_LOGV(x, ...) {if(g_debugLevel >= 4) {printf("V %s: ",x); printf(__VA_ARGS__); printf("\n");}}



