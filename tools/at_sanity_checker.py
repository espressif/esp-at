#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import sys
import csv

def ESP_LOGI(x):
    print(f'\033[32m{x}\033[0m')

def ESP_LOGE(x):
    sys.stderr.write(f'\033[31m{x}\n\033[0m')

repo_dir = os.path.dirname(os.path.dirname(__file__))

def at_check_partition_sanity(file):
    if not os.path.exists(file):
        ESP_LOGE(f'Partition file {file} does not exist.')
        return False
    with open(file, newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            if not row or row[0].startswith('#'):
                continue
            if len(row) >= 3 and row[1].strip() == 'data' and row[2].strip() == '0x22':
                return True
    ESP_LOGE(f'Partition table: {file} does not contain a valid partition for compress ota which type is "data" and subtype is 0x22.\n'
             f'Please modify {file} if you want to use v2 bootloader_support_plus, or '
             f'remove `CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT` in *.defaults if you want to use v3 bootloader_support_plus.')
    return False

def at_check_compress_ota_sanity(partition_base_dir):
    ret = True
    at_compress_ota = False
    at_compress_ota_v2 = None
    partition_suffix = None
    with open(os.path.join(repo_dir, 'sdkconfig'), 'r') as f:
        data = f.read()
        # remove the duplicate lines
        dup_data = data.split('\n')
        uni_data = []
        for item in dup_data:
            if item not in uni_data or item.startswith('#'):
                uni_data.append(item)
        data = '\n'.join(uni_data)
        data = '[saved-args]' + data
        import configparser
        config = configparser.ConfigParser()
        config.optionxform = str
        config.read_string(data)

        for section in config.sections():
            for key, value in config.items(section):
                if key == 'CONFIG_BOOTLOADER_COMPRESSED_ENABLED':
                    at_compress_ota = True
                elif key == 'CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT':
                    at_compress_ota_v2 = True
                elif key == 'CONFIG_PARTITION_TABLE_FILENAME':
                    partition_suffix = value.strip('"')
    if at_compress_ota and at_compress_ota_v2:
        ret = at_check_partition_sanity(os.path.join(partition_base_dir, partition_suffix))

    return ret

def main():
    if len(sys.argv) != 2:
        raise Exception(f'Usage: {sys.argv[0]} <partition_base_dir>')
    partition_base_dir = sys.argv[1]

    if not at_check_compress_ota_sanity(partition_base_dir):
        sys.exit(1)

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        ESP_LOGE(f'A fatal error occurred: {e}')
        sys.exit(2)
