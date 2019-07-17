#ifndef __AT_API_H__
#define __AT_API_H__

#ifdef CONFIG_AT_OTA_SUPPORT
#include "at_upgrade.h"
#endif

#include "at_config.h"

#define ESP_AT_FACTORY_PARAMETER_SIZE       4096

#ifdef CONFIG_AT_OTA_SUPPORT
const char* esp_at_get_ota_token_by_id(uint32_t id, esp_at_ota_mode_type ota_mode);
#endif
const char* esp_at_get_module_name_by_id(uint32_t id);
uint32_t esp_at_get_module_id(void);
uint32_t esp_at_factory_parameter_init(void);
#endif
