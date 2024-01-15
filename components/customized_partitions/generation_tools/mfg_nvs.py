#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os, re, sys, subprocess, argparse

def ESP_LOGE(x):
    print('\033[31m{}\033[0m'.format(x))

def at_parse_size(size):
    size = size.strip()
    if size.startswith('0x'):
        return size
    size = size.upper()
    if not size.endswith('B'):
        size = size + 'B'
    units = {'B': 1, 'KB': 2**10, 'MB': 2**20, 'GB': 2**30, 'TB': 2**40}
    if not re.match(r' ', size):
        size = re.sub(r'([KMGT]?B)', r' \1', size)
    if not re.search(r'([KMGT]?B)', size):
        size += ' B'
    number, unit = [string.strip() for string in size.split()]
    return int(float(number) * units[unit])

def at_get_all_files(dir):
    all_files = []
    if os.path.isfile(dir):
        all_files.append(os.path.abspath(dir))
    for root, dirs, files in os.walk(dir):
        for f in files:
            if f.endswith('.rst') or f.endswith('.md'):
                all_files.append(os.path.join(root, f))
    return all_files

def get_to_read_config_dir(at_path):
    dependency = os.path.join(at_path, 'components', 'customized_partitions', 'at_customized_config_dependency.txt')
    sdkconfig = os.path.join(at_path, 'sdkconfig')

    to_read_config_name = []
    with open(sdkconfig) as f:
        sdkconfig_data = f.read()
    with open(dependency) as f:
        for line in f.readlines():
            line = line.strip()
            line = re.sub(' +', ' ', line)
            if line.startswith('#'):
                continue

            line = line.split()
            if (sdkconfig_data.find(''.join(['CONFIG_',line[0]])) != -1):
                cfg_dir = os.path.join(at_path, 'components', 'customized_partitions', 'raw_data', line[1])
                if os.path.exists(cfg_dir):
                    to_read_config_name.append(cfg_dir)
    return to_read_config_name

def create_mfg_csv(args):
    to_read_cfg_dir = get_to_read_config_dir(args.project_path)

    # create a new mfg_nvs.csv namespace and write header
    mfg_csv = os.path.join(args.outdir, args.partition_name + '.csv')
    with open(mfg_csv, 'w') as f:
        f.write('key,type,encoding,value\n')

    with open(mfg_csv, 'a') as f:
        for cfg_dir in to_read_cfg_dir:
            index = 0
            for cfg_file in sorted(os.listdir(cfg_dir)):
                if cfg_file.endswith('.crt') or cfg_file.endswith('.key') or cfg_file.endswith('.pem'):
                    if index == 0:
                        f.write(os.path.basename(cfg_dir) + ',namespace,,\n')
                    if len(os.listdir(cfg_dir)) == 1:
                        key = os.path.basename(cfg_dir)
                    else:
                        key = os.path.basename(cfg_dir) + '.' + str(index)
                        index = index + 1
                    value = os.path.join(cfg_dir, cfg_file)
                    f.write(key + ',file,binary,' + value + '\n')

def create_factory_param_csv(args):
    import json, csv
    with open(os.path.join(args.project_path, 'build', 'module_info.json')) as f:
        info = json.load(f)
        platform_name = info['platform']
        module_name = info['module']

    fdata = os.path.join(args.project_path, 'components', 'customized_partitions', 'raw_data', 'factory_param', 'factory_param_data.csv')
    ftype = os.path.join(args.project_path, 'components', 'customized_partitions', 'raw_data', 'factory_param', 'factory_param_type.csv')

    to_read_type_items = []
    with open(ftype) as f:
        csv_data_type = csv.reader(f)
        l_csv_data_type = list(csv_data_type)
        for item in l_csv_data_type:
            if len(item) == 3:
                to_read_type_items.append(item)

    to_read_data_items = []
    with open(fdata) as f:
        csv_data = csv.reader(f)
        csv_data = list(csv_data)
        for item in csv_data:
            if item[0] == 'platform':
                to_read_data_items.append(item)
                continue
            if item[0] == platform_name and item[1] == module_name:
                to_read_data_items.append(item)
                continue

    to_write_csv_items = []
    for to_find_item in to_read_type_items:
        has_data = 0
        pos = 0
        for data_item in to_read_data_items[0]:
            if to_find_item[0] == data_item:
                has_data = 1
                value = [row[pos] for row in to_read_data_items]
                if to_find_item[2] == 'string':
                    to_write_csv_items.append(','.join(to_find_item) + ',' + '"' + value[1] + '"')
                else:
                    to_write_csv_items.append(','.join(to_find_item) + ',' + value[1])
            pos = pos + 1
        if has_data == 0:
            raise Exception('No data found for {} in {}'.format(to_find_item, fdata))

    mfg_csv = os.path.join(args.outdir, args.partition_name + '.csv')
    with open(mfg_csv, 'a') as f:
        f.write('factory_param' + ',namespace,,\n')
        for item in to_write_csv_items:
            f.write(item + '\n')

def create_ble_data_csv(args):
    import csv
    fdata = os.path.join(args.project_path, 'components', 'customized_partitions', 'raw_data', 'ble_data', 'gatts_data.csv')
    if os.path.dirname(fdata) not in get_to_read_config_dir(args.project_path):
        return

    to_read_data_items = []
    with open(fdata) as f:
        csv_data = csv.reader(f)
        csv_data = list(csv_data)
        for item in csv_data:
            if not item or item[0].startswith('#'):
                continue
            to_read_data_items.append(item)

    to_write_csv_items = []
    cfg_idx = 0
    for data_item in to_read_data_items:
        data = 'cfg' + str(cfg_idx) + ',data,string,' + '"' + ','.join(data_item) + '"'
        cfg_idx = cfg_idx + 1
        to_write_csv_items.append(data)

    mfg_csv = os.path.join(args.outdir, args.partition_name + '.csv')
    with open(mfg_csv, 'a') as f:
        f.write('ble_data' + ',namespace,,\n')
        for item in to_write_csv_items:
            f.write(item + '\n')

def generate_mfg_bin(args):
    if sys.platform == 'win32':
        sys_python_path = sys.executable
    else:
        if os.environ.get('IDF_PYTHON_ENV_PATH') is None:
            sys_python_path = 'python'
        else:
            sys_python_path = os.path.join(os.environ.get('IDF_PYTHON_ENV_PATH'), 'bin', 'python')

    nvs_tool = os.path.join(args.project_path, 'esp-idf', 'components', 'nvs_flash', 'nvs_partition_generator', 'nvs_partition_gen.py')
    mfg_csv = os.path.join(args.outdir, args.partition_name + '.csv')
    mfg_bin = os.path.join(args.outdir, args.partition_name + '.bin')

    cmd = '{} {} generate {} {} {}'.format(sys_python_path, nvs_tool, mfg_csv, mfg_bin, at_parse_size(args.partition_size))
    ret = subprocess.call(cmd, shell = True)
    if ret or not os.path.exists(mfg_bin):
        raise Exception('{}'.format(cmd))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--partition_name', default='', help='partition name')
    parser.add_argument('--partition_size', default='', help='partition size')
    parser.add_argument('--outdir', default='', help='out dir')
    parser.add_argument('--project_path', default='', help='project path')
    args = parser.parse_args()

    create_mfg_csv(args)
    create_factory_param_csv(args)
    create_ble_data_csv(args)

    generate_mfg_bin(args)

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        ESP_LOGE('Failed to generate mfg_nvs.csv: {}'.format(e))
        sys.exit(1)
