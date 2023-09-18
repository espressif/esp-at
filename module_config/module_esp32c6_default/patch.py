#!/usr/bin/env python
#
# Copyright 2022 Espressif Systems (Shanghai) PTE LTD
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

def ESP_LOGI(x):
    print('\033[32m{}\033[0m'.format(x))

def ESP_LOGE(x):
    print('\033[31m{}\033[0m'.format(x))

def main():
    py_ads_dir = os.path.abspath(__file__)
    patch_abs_dir = os.path.join(os.path.dirname(py_ads_dir), 'patch')
    cur_abs_dir = os.getcwd()

    patch_list = {
        'blufi-adv.patch': os.path.join(cur_abs_dir, 'esp-idf'),
        'dhcps.patch': os.path.join(cur_abs_dir, 'esp-idf')
    }

    for name, path in patch_list.items():
        patch_abs_path = os.path.join(patch_abs_dir, name)
        if os.path.exists(patch_abs_path):
            if sys.platform == 'win32':
                cmd = 'cd {} && git apply --check {} 2> /COM && cd {}'.format(path, patch_abs_path, cur_abs_dir)
            else:
                cmd = 'cd {} && git apply --check {} 2> /dev/null && cd {}'.format(path, patch_abs_path, cur_abs_dir)

            ret = subprocess.call(cmd, shell = True)
            if ret:
                ESP_LOGI('{} does not need to be applied'.format(name))
            else:
                cmd = 'cd {} && git apply {} && cd {}'.format(path, patch_abs_path, cur_abs_dir)
                ret = subprocess.call(cmd, shell = True)
                if ret:
                    raise Exception('{} apply failed'.format(name))

        else:
            raise Exception('{} does not exist'.format(name))

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        ESP_LOGE('A fatal error occurred: {}'.format(e))
        sys.exit(2)
