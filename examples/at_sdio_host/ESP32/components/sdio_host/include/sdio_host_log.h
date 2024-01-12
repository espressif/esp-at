/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern int sdio_debugLevel;

#define SDIO_LOGE(x, ...) {if(sdio_debugLevel >= 0) {printf("E %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define SDIO_LOGW(x, ...) {if(sdio_debugLevel >= 1) {printf("W %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define SDIO_LOGI(x, ...) {if(sdio_debugLevel >= 2) {printf("I %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define SDIO_LOGD(x, ...) {if(sdio_debugLevel >= 3) {printf("D %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define SDIO_LOGV(x, ...) {if(sdio_debugLevel >= 4) {printf("V %s: ",x); printf(__VA_ARGS__); printf("\n");}}

#ifdef __cplusplus
}
#endif
