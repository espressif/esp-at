#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import xlrd
import csv
import io
import binascii
import os
import sys
import argparse
from ctypes import *

ESP_AT_FACTORY_PARAM_SIZE = 4096
# param_type_dicts = {}
# param_data_lists = []

def get_param_type_info(source_file, sheet_name):
    filename,filetype = os.path.splitext(source_file)

    param_type_dicts = {}
    if filetype == '.xlsx':
        data = xlrd.open_workbook(source_file)
        sheet = data.sheet_by_name(sheet_name)
        headers = sheet.row_values(0)

        for row in range(1,sheet.nrows):
            dict = {}
            for col in range(1,sheet.ncols):
                dict[headers[col]] = sheet.row_values(row)[col]

            param_type_dicts[sheet.row_values(row)[0]] = dict
    elif filetype == '.csv':
        with open(source_file) as f:
            csv_data = csv.reader(f)
            data = list(csv_data)
            headers = data[0]

            for row_data in data[1:]: # skip header
                dict = {}
                for col in range(1, len(row_data)):
                    dict[headers[col]] = row_data[col]

                param_type_dicts[row_data[0]] = dict

    else:
        print('The file type is not supported.')
        exit()

    return param_type_dicts

def get_param_data_info(source_file, sheet_name):
    filename,filetype = os.path.splitext(source_file)
    if filetype == '.xlsx':
        data = xlrd.open_workbook(source_file)
        sheet = data.sheet_by_name(sheet_name)
        headers = sheet.row_values(0)

        for row in range(1,sheet.nrows):
            dict = {}
            for col in range(1,sheet.ncols):
                dict[headers[col]] = sheet.row_values(row)[col]

            param_data_dicts[sheet.row_values(row)[0]] = dict
    elif filetype == '.csv':
        with open(source_file) as f:
            csv_data = csv.reader(f)
            param_data_list = list(csv_data)

    else:
        print('The file type is not supported.')
        exit()

    return param_data_list

def generate_factory_param_bin(data_lists, type_dicts, target_name, platform, module, log_file):
    if os.path.exists(os.path.dirname(log_file)) == True:
        if os.path.exists(log_file) == True:
            os.remove(log_file)
    else:
        os.makedirs(os.path.dirname(log_file))

    headers = data_lists[0]

    nrows = len(data_lists)
    ncols = len(data_lists[0])

    factory_param_bin = (c_ubyte * ESP_AT_FACTORY_PARAM_SIZE)()
    has_parameter_file = 0
    platform_index = ncols
    module_name_index = ncols

    for i in range(ncols): # get platform index
        if headers[i] == 'platform':
            platform_index = i
            break

    for i in range(ncols): # get module name index
        if headers[i] == 'module_name':
            module_name_index = i
            break

    if platform_index == ncols:
        print('Not found platform in header.')
        exit()

    if module_name_index == ncols:
        print('Not found module name in header.')
        exit()

    for row in range(1,nrows): # skip header
        memset(byref(factory_param_bin),0xFF, len(factory_param_bin))
        data_list = data_lists[row]

        platform_name = data_list[platform_index]
        module_name = data_list[module_name_index]
        if platform_name.upper() != platform:
            continue

        if module_name.upper() != module:
            continue

        for col in range(0, ncols):
            type_dict = type_dicts.get(headers[col])
            if type_dict is None:
                continue

            if int(type_dict.get('size')) <= 0:
                continue

            memset(byref(factory_param_bin, int(type_dict.get('offset'))),0x0, int(type_dict.get('size')))

            if type_dict.get('type') == 'integer':
                value = 0

                cell_data = data_list[col]

                if type(cell_data).__name__ == 'unicode' or type(cell_data).__name__ == 'str':
                    if sys.version_info.major == 2:
                        str_value = cell_data.encode('utf-8')
                    else:
                        str_value = cell_data
                    if str_value.startswith(('0x', '0X')):
                        value = int(str_value,16)
                    elif str_value.startswith(('0')):
                        value = int(str_value,8)
                    else:
                        value = int(str_value,10)
                else:
                    value = cell_data

                c_data = c_int(int(value))
                memmove(byref(factory_param_bin, int(type_dict.get('offset'))), byref(c_data), int(type_dict.get('size')))
            else:
                cell_data = data_list[col]

                value = cell_data.encode('utf-8')
                c_data = create_string_buffer(value, int(type_dict.get('size')))
                memmove(byref(factory_param_bin, int(type_dict.get('offset'))), byref(c_data), int(type_dict.get('size')))

        target_bin_name = os.path.splitext(target_name)[0] + '_' + module_name + '.bin'
        with open(target_bin_name, 'wb+') as f:
            f.write(factory_param_bin)
            print('generate parameter bin: platform %s, module name %s'%(platform_name.upper(),module_name))
            with open(log_file, 'a+') as log_f:
                log_f.write('%s %s %s '%(module_name, os.path.basename(target_name), target_bin_name))

            if module_name == module:
                with open(target_name, 'wb+') as target_f:
                    target_f.write(factory_param_bin)
                    has_parameter_file = 1

    if has_parameter_file == 0:
        target_bin_name = os.path.splitext(target_name)[0] + '_' + module + '.bin'
        memset(byref(factory_param_bin),0xFF, len(factory_param_bin))
        with open(target_bin_name, 'wb+') as target_f:
            target_f.write(factory_param_bin)

        with open(target_name, 'wb+') as target_f:
                    target_f.write(factory_param_bin)

        with open(log_file, 'a+') as log_f:
                log_f.write('%s %s %s '%(module, os.path.basename(target_name), target_bin_name))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--platform', default='esp32', help='the chip platform')
    parser.add_argument('--module', default='Wroom32', help='the module name for espressif AT')
    parser.add_argument('--define_file', default='factory_param_type.csv', help='factory parameter file name')
    parser.add_argument('--module_file', default='factory_param_data.csv', help='factory parameter file name')
    parser.add_argument('--bin_name', default='factory_param.bin', help='factory parameter bin file name')
    parser.add_argument('--log_file', default='./factory_parameter.log', help='the file name stored the module name')
    args = parser.parse_args()

    module_file = args.module_file
    define_file = args.define_file

    if os.path.exists(define_file) == False:
        print('%s does not exist'%define_file)
        return

    if os.path.exists(module_file) == False:
        print('%s does not exist'%module_file)
        return

    param_type_dicts = get_param_type_info(define_file, 'Param_Type')
    param_data_lists = get_param_data_info(module_file, 'Param_Data')

    generate_factory_param_bin(param_data_lists, param_type_dicts, args.bin_name, args.platform.upper(), args.module, args.log_file)

if __name__ == '__main__':
    main()
