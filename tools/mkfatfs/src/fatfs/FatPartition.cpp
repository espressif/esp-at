// Copyright 2015-2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstring> // memset/memcpy
#include "esp_log.h"
#include "FatPartition.h"

static const char *TAG = "FatPartition";

std::vector<uint8_t> g_flashmem;


FatPartition::FatPartition(const esp_partition_t *partition)
{
    this->partition = partition;
}

size_t FatPartition::chip_size()
{
    return this->partition->size;
}

esp_err_t FatPartition::erase_sector(size_t sector)
{
    esp_err_t result = ESP_OK;
    result = erase_range(sector * SPI_FLASH_SEC_SIZE, SPI_FLASH_SEC_SIZE);
    return result;
}

esp_err_t FatPartition::erase_range(size_t start_address, size_t size)
{
    esp_err_t result = ESP_FAIL;
    if (g_flashmem.size() >= (start_address + size)) {
      result = ESP_OK;
      memset(&g_flashmem[0] + start_address, 0xff, size);
    }
    if (result == ESP_OK) {
	//The z portion is a length specifier which says the argument will be size_t in length.
        ESP_LOGV(TAG, "erase_range - start_address=0x%08zx, size=0x%08zx, result=0x%08x", start_address, size, result);
    } else {
        ESP_LOGE(TAG, "erase_range - start_address=0x%08zx, size=0x%08zx, result=0x%08x", start_address, size, result);
    }
    return result;
}

esp_err_t FatPartition::write(size_t dest_addr, const void *src, size_t size)
{
    esp_err_t result = ESP_FAIL;
    if (g_flashmem.size() >= (dest_addr + size)) {
      result = ESP_OK;
      memcpy(&g_flashmem[0] + dest_addr, src, size);
    }
    return result;
}

esp_err_t FatPartition::read(size_t src_addr, void *dest, size_t size)
{
    esp_err_t result = ESP_FAIL;
    if (g_flashmem.size() >= (src_addr + size)) {
      result = ESP_OK;
      memcpy(dest, &g_flashmem[0] + src_addr, size);
    }
    return result;
}

size_t FatPartition::sector_size()
{
    ESP_LOGI(TAG, "%s() returns sector_size=%d", __func__, SPI_FLASH_SEC_SIZE);
    return SPI_FLASH_SEC_SIZE;
}

FatPartition::~FatPartition()
{

}
