// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "esp_partition.h"

//MVA only satisfy declarations in esp_partition.h

esp_err_t esp_partition_read(const esp_partition_t* partition,
        size_t src_offset, void* dst, size_t size)
{
    return ESP_OK;
}

esp_err_t esp_partition_write(const esp_partition_t* partition,
                             size_t dst_offset, const void* src, size_t size)
{
    return ESP_OK;
}

esp_err_t esp_partition_erase_range(const esp_partition_t* partition,
                                    uint32_t start_addr, uint32_t size) //MVA size_t -> uint32_t
{
    return ESP_OK;
}



