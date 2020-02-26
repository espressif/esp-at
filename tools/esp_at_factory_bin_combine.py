#
# ESPRESSIF MIT License
#
# Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

import os
import re
import argparse

ESP_FLASH_MODE = {"QIO": 0, "QOUT": 1, "DIO": 2, "DOUT": 3, "FAST_READ": 4, "SLOW_READ": 5}
ESP_FLASH_SIZE = {"1MB": 0, "2MB": 1, "4MB": 2, "8MB": 3, "16MB": 4}
ESP_BIN_SIZE = {"1MB": 1024*1024, "2MB": 2*1024*1024, "4MB": 4*1024*1024, "8MB": 8*1024*1024, "16MB": 16*1024*1024}
ESP_FLASH_SPEED = {"40M": 0, "26M": 1, "20M": 2, "80M": 3}

def esp32_at_combine_bin(module, flash_mode, flash_size, flash_speed, build_dir, parameter_file, download_config):
    bin_data = bytearray([0xFF] * (ESP_BIN_SIZE[flash_size]))

    with open(download_config) as f:
        data = f.read()
        address_list = re.compile(r"0x[\da-fA-F]+ \S+").findall(data)

    bin_list = {}
    for i, address_pair in enumerate(address_list):
        bin_addr_str, bin_file = list(address_pair.split(' '))
        bin_addr = int(bin_addr_str, 16)
        print('0x%x,%s' % (bin_addr, bin_file))
        bin_list[os.path.basename(bin_file)] = bin_addr
        with open(os.path.join(build_dir, bin_file), 'rb') as f:
            data = f.read()
            for i, byte_data in enumerate(data):
                bin_data[bin_addr + i] = byte_data

    bootloader_addr = bin_list['bootloader.bin']
    bin_data[bootloader_addr + 2] = ESP_FLASH_MODE[flash_mode]  # Flash mode DIO
    # 0x20 Flash size 4MB, speed 40MHz
    bin_data[bootloader_addr + 3] = (ESP_FLASH_SIZE[flash_size] << 4) | ESP_FLASH_SPEED[flash_speed]

    if parameter_file != None:
        with open(parameter_file) as f:
            factory_parameter = f.read()
            module_name_list = re.compile(r"\S+ \S+ \S+").findall(factory_parameter)
            for i, module_name_pair in enumerate(module_name_list):
                module_name, default_name, bin_file = list(module_name_pair.split(' '))
                if module == module_name:
                    addr = bin_list[default_name]
                    with open(bin_file, 'rb') as f:
                        data = f.read()

                        for i, byte_data in enumerate(data):
                            bin_data[addr + i] = byte_data

                    factory_bin = os.path.join(os.path.dirname(parameter_file), 'factory_' + module_name + '.bin')
                    with open(factory_bin, 'wb') as f:
                        f.write(bin_data)
                        print("Create %s for %s finished"%(factory_bin, module_name))
    
    else:
        factory_bin = os.path.join(build_dir, 'factory.bin')
        with open(factory_bin, 'wb') as f:
            f.write(bin_data)
            print("Create %s finished"%(factory_bin))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--module_name", default="WROOM-32", help="The module name")
    parser.add_argument("--flash_mode", default="DIO", help="Flash mode: QIO,QOUT,DIO,DOUT,FAST_READ,SLOW_READ")
    parser.add_argument("--flash_size", default="4MB", help="Flash size: 1MB,2MB,4MB,8MB,16MB")
    parser.add_argument("--flash_speed", default="40M", help="Flash speed: 40M,26M,20M,80M")
    parser.add_argument("--bin_directory", default="build", help="build directory")
    parser.add_argument("--parameter_file", default=None, help="factory parameter file")
    parser.add_argument("--download_config", default='download.config', help="flash download config file")
    args = parser.parse_args()

    esp32_at_combine_bin(args.module_name.upper(), args.flash_mode.upper(), args.flash_size.upper(), 
        args.flash_speed.upper(), args.bin_directory, args.parameter_file, args.download_config)

if __name__ == '__main__':
    main()
