#
# ESPRESSIF MIT License
#
# Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
#
# Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
# it is free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished
# to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
#

import xlrd
import csv
import io
import binascii
import os
import sys
import argparse
from ctypes import *

ESP_AT_FACTORY_PARAM_SIZE = 4096
param_type_dicts = {}

def get_param_type_info(source_file, sheet_name):
    filename,filetype = os.path.splitext(source_file)
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

            for row_data in data[1:]:
                dict = {}
                for col in range(1, len(row_data)):
                    dict[headers[col]] = row_data[col]

                param_type_dicts[row_data[0]] = dict

    else:
        print("The file type is not supported.")
        exit()

def generate_factory_param_bin(source_file, sheet_name, target_name, module, log_file):
    if os.path.exists(os.path.dirname(log_file)) == True:
        if os.path.exists(log_file) == True:
            os.path.remove(log_file)
    else:
        os.makedirs(os.path.dirname(log_file))

    filename,filetype = os.path.splitext(source_file)
    if filetype == '.xlsx':
        data = xlrd.open_workbook(source_file)
        sheet = data.sheet_by_name(sheet_name)
        nrows = sheet.nrows
        ncols = sheet.ncols
    elif filetype == '.csv':
        with open(source_file) as f:
            csv_data = csv.reader(f)
            data = list(csv_data)

            nrows = len(data)
            ncols = len(data[0])
    else:
        print("The file type is not supported.")
        exit()

    factory_param_bin = (c_ubyte * ESP_AT_FACTORY_PARAM_SIZE)()
    has_parameter_file = 0
    for row in range(1,nrows):
        memset(byref(factory_param_bin),0xFF, len(factory_param_bin))

        if filetype == '.xlsx':
            module_name = sheet.row_values(row)[0]
        elif filetype == '.csv':
            module_name = data[row][0]

        for col in range(1, ncols):
            if filetype == '.xlsx':
                dict = param_type_dicts.get(sheet.row_values(0)[col])
            elif filetype == '.csv':
                dict = param_type_dicts.get(data[0][col])

            if int(dict.get('size')) == -1:
                pass

            memset(byref(factory_param_bin, int(dict.get('offset'))),0x0, int(dict.get('size')))

            if dict.get('type') == 'integer':
                value = 0

                if filetype == '.xlsx':
                    cell_data = sheet.row_values(row)[col]
                elif filetype == '.csv':
                    cell_data = data[row][col]

                if type(cell_data).__name__ == 'unicode' or type(cell_data).__name__ == 'str':
                    str_value = cell_data.encode('utf-8')
                    if str_value.startswith(('0x', '0X')):
                        value = int(str_value,16)
                    elif str_value.startswith(('0')):
                        value = int(str_value,8)
                    else:
                        value = int(str_value,10)
                else:
                    value = cell_data

                c_data = c_int(int(value))
                memmove(byref(factory_param_bin, int(dict.get('offset'))), byref(c_data), int(dict.get('size')))
            else:
                if filetype == '.xlsx':
                    cell_data = sheet.row_values(row)[col]
                elif filetype == '.csv':
                    cell_data = data[row][col]

                value = cell_data.encode('utf-8')
                c_data = create_string_buffer(value, int(dict.get('size')))
                memmove(byref(factory_param_bin, int(dict.get('offset'))), byref(c_data), int(dict.get('size')))

        target_bin_name = os.path.splitext(target_name)[0] + '_' + module_name + '.bin'
        with open(target_bin_name, 'wb+') as f:
            f.write(factory_param_bin)
            with open(log_file, 'a+') as log_f:
                log_f.write("%s %s %s "%(module_name, os.path.basename(target_name), target_bin_name))

            if module_name == module:
                with open(target_name, 'wb+') as target_f:
                    target_f.write(factory_param_bin)
                    has_parameter_file = 1

    if has_parameter_file == 0:
        target_bin_name = os.path.splitext(target_name)[0] + '_' + target_name + '.bin'
        with open(target_name, 'wb+') as target_f:
            memset(byref(factory_param_bin),0xFF, len(factory_param_bin))
            target_f.write(factory_param_bin)

        with open(log_file, 'a+') as log_f:
                log_f.write("%s %s %s "%(target_name, os.path.basename(target_name), target_bin_name))    

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--module", default="Wroom32", help="the module name for espressif AT")
    parser.add_argument("--define_file", default="factory_param_type.csv", help="factory parameter file name")
    parser.add_argument("--module_file", default="factory_param_data.csv", help="factory parameter file name")
    parser.add_argument("--bin_name", default="factory_param.bin", help="factory parameter bin file name")
    parser.add_argument("--log_file", default="./factory_parameter.log", help="the file name stored the module name")
    args = parser.parse_args()

    module_file = args.module_file
    define_file = args.define_file

    if os.path.exists(define_file) == False:
        print('%s does not exist'%define_file)
        return

    if os.path.exists(module_file) == False:
        print('%s does not exist'%module_file)
        return

    get_param_type_info(define_file, 'Param_Type')
    generate_factory_param_bin(module_file, 'Param_Data', args.bin_name, args.module, args.log_file)

if __name__ == '__main__':
    main()

