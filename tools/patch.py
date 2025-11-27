#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import sys
import subprocess
import shutil
import filecmp
import configparser

def ESP_LOGI(x):
    print(f'\033[32m{x}\033[0m')

def ESP_LOGE(x):
    sys.stderr.write(f'\033[31m{x}\n\033[0m')

at_patch_defaults = {
    'path': 'esp-idf',
    'target': ['esp32c2', 'esp32c3', 'esp32c5', 'esp32c6', 'esp32c61', 'esp32', 'esp32s2'],
    'when': 'before_sdkconfig',
    'dependency': None,
}
repo_dir = os.path.dirname(os.path.dirname(__file__))

def has_dependency(dependency, with_sdkconfig):
    # check if this dependency is in sdkconfig
    if with_sdkconfig:
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
                    if key == dependency:
                        return True

    # check if this dependency is in environment
    if os.environ.get(dependency):
        return True
    return False

def main():
    if len(sys.argv) != 4:
        raise Exception(f'Usage: {sys.argv[0]} <path> <target> <when>')

    patch_dir = sys.argv[1]
    target = sys.argv[2]
    when = sys.argv[3]

    ini_file = os.path.join(patch_dir, 'patch_list.ini')
    if not os.path.exists(ini_file):
        return

    config = configparser.ConfigParser()
    config.read(ini_file)

    for section in config.sections():
        path_option = config.get(section, 'path') if config.has_option(section, 'path') else at_patch_defaults['path']

        # check if the patch needs to be applied to the current target
        target_option = config.get(section, 'target') if config.has_option(section, 'target') else at_patch_defaults['target']
        if target not in target_option:
            continue

        # check if the patch needs to be applied at this time
        when_option = config.get(section, 'when') if config.has_option(section, 'when') else at_patch_defaults['when']
        if when != when_option:
            continue

        # check if the patch has dependencies and if they are met
        if config.has_option(section, 'dependency'):
            dependencies_option = config.get(section, 'dependency')
        else:
            dependencies_option = at_patch_defaults['dependency']
        dependency = True
        if dependencies_option:
            with_sdkconfig = True if when == 'after_sdkconfig' else False
            dependencies_option = dependencies_option.split(', ')
            for dep in dependencies_option:
                dep = dep.strip("'")
                if not has_dependency(dep, with_sdkconfig):
                    dependency = False
                    break
        if not dependency:
            continue

        patch_name = section.strip('[]')
        src_patch_path = os.path.join(patch_dir, patch_name)
        dst_patch_path = os.path.join(repo_dir, path_option)

        if not os.path.exists(src_patch_path) or not os.path.exists(dst_patch_path):
            raise Exception(f'{src_patch_path} or {dst_patch_path} does not exist')

        # avoid applying the same patch multiple times
        dst_patch_file = os.path.join(repo_dir, path_option, patch_name)
        if os.path.exists(dst_patch_file):
            ESP_LOGI(f'{patch_name} already exists, skipping.')
            continue
        else:
            shutil.copy(src_patch_path, dst_patch_path)

        # *.patch
        cur_dir = os.getcwd()
        if patch_name.endswith('.patch'):
            cmd = f'cd {dst_patch_path} && git apply --check {src_patch_path} && cd {cur_dir}'
            ret = subprocess.run(cmd, capture_output = True, shell = True)
            if ret.returncode:
                ESP_LOGI(f'{patch_name} does not need to be applied.')
            else:
                cmd = f'cd {dst_patch_path} && git apply {src_patch_path} && cd {cur_dir}'
                ret = subprocess.call(cmd, shell = True)
                if ret:
                    raise Exception(f'{patch_name} apply failed.')
                ESP_LOGI(f'{patch_name} has been applied.')
        # *.a
        elif patch_name.endswith('.a'):
            if filecmp.cmp(dst_patch_path, src_patch_path):
                ESP_LOGI(f'{src_patch_path} does not need to be applied.')
            else:
                shutil.copy(src_patch_path, dst_patch_path)
                ESP_LOGI(f'{src_patch_path} has been applied.')
        # directory
        elif os.path.isdir(src_patch_path):
            shutil.copytree(src_patch_path, dst_patch_path, dirs_exist_ok=True)
            ESP_LOGI(f'Directory {src_patch_path} has been synchronized to {dst_patch_path}.')

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        ESP_LOGE(f'A fatal error occurred: {e}')
        sys.exit(2)
