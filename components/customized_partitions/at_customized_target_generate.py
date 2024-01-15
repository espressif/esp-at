#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import argparse
import re

to_read_config_name = {}
to_write_partition = {}

def get_to_read_config_name(dependency_file, sdkconfig_file):
    with open(sdkconfig_file) as sdkconfig_f:
        sdkconfig = sdkconfig_f.read()
        with open(dependency_file) as f:
            for line in f.readlines():
                line_str = line.strip()
                line_str = re.sub(' +', ' ', line_str)
                if not line_str.startswith('#'):
                    str_list = line_str.split()
                    if (sdkconfig.find(''.join(['CONFIG_',str_list[0]])) != -1):
                        to_read_config_name.update({str_list[1] : ''})

def get_to_write_partition(partition_file):
    with open(partition_file) as f:
        for line in f.readlines():
            line_str = line.strip()
            line_str = re.sub(' +', '', line_str)
            if not line_str.startswith('#'):
                str_list = line_str.split(',')
                if str_list[0] in to_read_config_name:
                    offset = str_list[3]
                    size = str_list[4]
                    to_write_partition.update({str_list[0] : {'offset' : offset, 'size' : size}})

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--dependency_file', help='dependency')
    parser.add_argument('--sdkconfig_file', help='sdkconfig')
    parser.add_argument('--partition_file', help='at_customize.csv')
    parser.add_argument('--output_dir', default='output', help='the output bin directory')
    parser.add_argument('--flash_args_file', default='flash_args_file', help='the file to store flash args')
    args = parser.parse_args()
    sdkconfig_file = args.sdkconfig_file
    dependency_file = args.dependency_file
    partition_file = args.partition_file
    output_dir = args.output_dir
    flash_args_file = args.flash_args_file
    get_to_read_config_name(dependency_file, sdkconfig_file)
    get_to_write_partition(partition_file)

    if os.path.exists(output_dir) == False:
        os.mkdir(output_dir)

    with open(flash_args_file, 'w+') as args_file:
        for partition in to_write_partition:
            args_file.write(''.join([to_write_partition[partition]['offset'], ' ']))
            args_file.write(''.join([os.path.join(output_dir, partition), ''.join(['.bin', ' '])]))
            args_file.write(''.join([to_write_partition[partition]['size'], '\r\n']))

if __name__ == '__main__':
    main()
