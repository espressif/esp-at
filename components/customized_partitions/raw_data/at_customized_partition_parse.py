#
# ESPRESSIF MIT License
#
# Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
import os
import sys
import stat
import argparse
import subprocess

generation_tools = {}

def parse_generation_tools (dependency_file, sdkconfig_file):
    with open(sdkconfig_file) as sdkconfig_f:
        sdkconfig = sdkconfig_f.read()
        with open(dependency_file) as f:
            for line in f.readlines():
                line_str = line.strip()
                if not line_str.startswith("#"):
                    str_list = line_str.split()
                    if (sdkconfig.find("".join(["CONFIG_",str_list[0]])) != -1):
                        generation_tools.update({str_list[1] : ""})

def parse_partition_tables (partition_file):
    with open(partition_file) as f:
        for line in f.readlines():
            line_str = line.strip()
            if not line_str.startswith("#"):
                str_list = line_str.split(',')
                if str_list[0] in generation_tools:
                    offset = str_list[3]
                    size = str_list[4]
                    generation_tools.update({str_list[0] : {'offset' : offset, 'size' : size}})

def main():
    """ main """
    parser = argparse.ArgumentParser()
    parser.add_argument("--dependency_file", help="dependency")
    parser.add_argument("--sdkconfig_file", help="sdkconfig")
    parser.add_argument("--partition_file", help="at_customize.csv")
    parser.add_argument("--output_dir", default="output", help="the output bin directory")
    parser.add_argument("--flash_args_file", default="flash_args_file", help="the file to store flash args")
    args = parser.parse_args()

    sdkconfig_file = args.sdkconfig_file
    dependency_file = args.dependency_file
    partition_file = args.partition_file
    output_dir = args.output_dir
    flash_args_file = args.flash_args_file
    parse_generation_tools(dependency_file, sdkconfig_file)
    parse_partition_tables(partition_file)

    if os.path.exists(output_dir) == False:
        os.mkdir(output_dir)

    with open(flash_args_file, 'w+') as args_file:
        for partition in generation_tools:
            args_file.write(''.join([generation_tools[partition]['offset'], ' ']))
            args_file.write(''.join([os.path.join(output_dir, partition), ''.join(['.bin', ' '])]))
            args_file.write(''.join([generation_tools[partition]['size'], '\r\n']))

if __name__ == '__main__':
    main()