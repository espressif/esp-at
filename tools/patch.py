#!/usr/bin/env python
#
# Copyright 2023 Espressif Systems (Shanghai) PTE LTD
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# WARNING: we don't check for Python build-time dependencies until
# check_environment() function below. If possible, avoid importing
# any external libraries here - put in external script, or import in
# their specific function instead.

import os
import sys
import subprocess
import shutil
import filecmp
import configparser

def ESP_LOGI(x):
    print('\033[32m{}\033[0m'.format(x))

def ESP_LOGE(x):
    print('\033[31m{}\033[0m'.format(x))

def main():
    if len(sys.argv) != 2:
        raise Exception('Usage: {} <path>'.format(sys.argv[0]))

    module_dir = sys.argv[1]
    ini_file = os.path.join(module_dir, 'patch', 'patch_list.ini')
    if not os.path.exists(ini_file):
        return

    config = configparser.ConfigParser()
    config.read(ini_file)

    for section in config.sections():
        patch_name = section.strip('[]')
        src_patch_path = os.path.join(module_dir, 'patch', patch_name)
        dst_patch_path = os.path.join(os.getcwd(), config.get(section, 'path'))

        if not os.path.exists(src_patch_path) or not os.path.exists(dst_patch_path):
            raise Exception('{} or {} does not exist'.format(src_patch_path, dst_patch_path))

        if patch_name.endswith('.patch'):
            cmd = 'cd {} && git apply --check {} && cd {}'.format(dst_patch_path, src_patch_path, os.getcwd())
            ret = subprocess.run(cmd, capture_output = True, shell = True)
            if ret.returncode:
                ESP_LOGI('{} does not need to be applied.'.format(patch_name))
            else:
                cmd = 'cd {} && git apply {} && cd {}'.format(dst_patch_path, src_patch_path, os.getcwd())
                ret = subprocess.call(cmd, shell = True)
                if ret:
                    raise Exception('{} apply failed.'.format(patch_name))
                ESP_LOGI('{} has been applied'.format(patch_name))

        elif patch_name.endswith('.a'):
            if filecmp.cmp(dst_patch_path, src_patch_path):
                ESP_LOGI('{} does not need to be applied.'.format(src_patch_path))
            else:
                shutil.copy(src_patch_path, dst_patch_path)
                ESP_LOGI('{} has been applied.'.format(src_patch_path))

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        ESP_LOGE('A fatal error occurred: {}'.format(e))
        sys.exit(2)
