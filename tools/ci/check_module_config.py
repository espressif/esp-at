#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0
import os
import sys

repo_dir = os.getcwd()

def ESP_LOGI(x):
    print('\033[32m{}\033[0m'.format(x))

def ESP_LOGE(x):
    sys.stderr.write(f'\033[31m{x}\n\033[0m')

def at_get_defaults_list(path):
    defaults = []
    for root, _, files in os.walk(path):
        for file in files:
            if file.endswith('.defaults'):
                defaults.append(os.path.abspath(os.path.join(root, file)))
    return defaults

def at_check_defaults_file(filepath):
    seen_keys = set()
    with open(filepath, 'r', encoding='utf-8') as f:
        for line_number, line in enumerate(f, 1):
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            if '=' not in line:
                continue
            key = line.split('=')[0].strip()
            if key in seen_keys:
                ESP_LOGE(f'Duplicate key <{key}> found in {filepath}:{line_number}.')
                return False
            seen_keys.add(key)
    return True

def at_check_module_config():
    defaults_files = at_get_defaults_list(os.path.join(repo_dir, 'module_config'))
    if not defaults_files:
        ESP_LOGE('No *.defaults files found in the repository.')
        sys.exit(1)
    for defaults_file in defaults_files:
        if at_check_defaults_file(defaults_file) is False:
            return False
    return True

if __name__ == '__main__':
    if at_check_module_config():
        ESP_LOGI('Module configuration check completed successfully.')
    else:
        ESP_LOGE('Module configuration check failed.')
        sys.exit(1)
