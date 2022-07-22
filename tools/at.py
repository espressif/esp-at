#!/usr/bin/env python
#
# ESPRESSIF MIT License
#
# Copyright (c) 2022 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
#
# Permission is hereby granted for use on ESPRESSIF SYSTEMS only, in which case,
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

import argparse
import re
import os
import sys
from shutil import copyfile
from struct import Struct

__version__ = "v1.0-dev"

# To locate the starting position of the parameter partition
parameter_pattern = re.compile(b'\xFC\xFC[\x03|\x02|\x01]')

sec_size = 4096
min_firmware_size = (1024 * 1024)
para_partition_size = (4 * 1024)

def ESP_LOGI(x):
    print("\033[32m{}\033[0m".format(x))

def ESP_LOGE(x):
    print("\033[31m{}\033[0m".format(x))

def arg_auto_int(x):
    r = int(x, 0)
    return r if r >= 0 else -1

def at_read_records(format, f):
    record_struct = Struct(format)
    chunks = f.read(record_struct.size)
    return (record_struct.unpack(chunks))

def at_write_records(records, format, f):
    record_struct = Struct(format)
    x = record_struct.pack(*records)
    f.write(x)

def at_parameter_assign_int(arg, fixed_len, l, lidx):
    if arg:
        l[lidx] = arg & (256 ** fixed_len - 1) if arg >= 0 else -1

def at_parameter_assign_str(arg, fixed_len, l, lidx):
    if arg:
        larg = list(arg)
        larg = larg + ['\x00'] * (fixed_len - len(larg))
        arg_tmp = [x.encode() for x in larg]
        l[lidx : (lidx+fixed_len)] = arg_tmp[0 : fixed_len]

def modify_bin(esp, args):
    print(args)

    if not os.path.exists(args.input):
        ESP_LOGE("File does not exist: {}".format(args.input))
        sys.exit(2)
    fsize = os.path.getsize(args.input)
    if (fsize != para_partition_size) and ((fsize % min_firmware_size) or (fsize / min_firmware_size > 16)):
        ESP_LOGE("Invalid file size: {}".format(fsize))
        sys.exit(2)

    copyfile(args.input, args.output)

    # find out the parameter start address
    # prefer to use input configuation
    if args.parameter_offset:
        param_addr = args.parameter_offset
    else:
        with open(args.output, "rb") as fp:
            data = fp.read()
            try:
                param_addr = re.search(parameter_pattern, data).span()[0]
            except Exception as e:
                ESP_LOGE("Can not find valid entry of parameter partition, please check firmware: {}".format(args.input))
                sys.exit(2)

    if param_addr % sec_size != 0:
        ESP_LOGE("Found wrong entry of parameter partition: {}, please manually specify \"--parameter_offset\" parameter!".format(hex(param_addr)))
        sys.exit(2)
    else:
        ESP_LOGI("factory parameter entry address: {}".format(hex(param_addr)))

    # modify parameter
    with open(args.output, "rb+") as fp:
        param_format = '<HBBBbBB 4c i bbbbbbH 32c 32c'     # valid 88 bytes and 4008 padding bytes
        fp.seek(param_addr, 0)
        raw_at_parameter = at_read_records(param_format, fp)
        print("raw parameters: {}\r\n".format(raw_at_parameter))
        list_at_parameter = list(raw_at_parameter)

        """
        <_magic_code>, <_version>, <_rsvd>, <tpower>, <uart_x>, <schan>, <nchan>, <country code>, <uart baud>,
        <tx_pin>, <tx_pin>, <cts>, <rts>, <txctrl>, <rxctrl>, <_rsvd>, <platform>, <module name>
        """
        at_parameter_assign_int(args.tx_power, 1, list_at_parameter, 3)
        at_parameter_assign_int(args.uart_num, 1, list_at_parameter, 4)
        at_parameter_assign_int(args.start_channel, 1, list_at_parameter, 5)
        at_parameter_assign_int(args.channel_number, 1, list_at_parameter, 6)
        at_parameter_assign_str(args.country_code, 4, list_at_parameter, 7)
        at_parameter_assign_int(args.baud, 4, list_at_parameter, 11)
        at_parameter_assign_int(args.tx_pin, 1, list_at_parameter, 12)
        at_parameter_assign_int(args.rx_pin, 1, list_at_parameter, 13)
        at_parameter_assign_int(args.cts_pin, 1, list_at_parameter, 14)
        at_parameter_assign_int(args.rts_pin, 1, list_at_parameter, 15)
        at_parameter_assign_int(args.tx_control_pin, 1, list_at_parameter, 16)
        at_parameter_assign_int(args.rx_control_pin, 1, list_at_parameter, 17)
        at_parameter_assign_str(args.platform, 32, list_at_parameter, 19)
        at_parameter_assign_str(args.module_name, 32, list_at_parameter, 51)
        new_at_parameter = tuple(list_at_parameter)

        fp.seek(param_addr, 0)
        at_write_records(new_at_parameter, param_format, fp)

        fp.seek(param_addr, 0)
        raw_at_parameter = at_read_records(param_format, fp)
        print("new parameters: {}\r\n".format(raw_at_parameter))

        ESP_LOGI("New esp-at firmware successfully generated! ----> {}".format(os.path.abspath(args.output)))

def generate_bin(esp, args):
    print("TODOs: ESP-AT will add this feature in v2.4.0.0+")

def version(esp, args):
    print(__version__)

def main(argv=None, esp=None):
    parser = argparse.ArgumentParser(description='at.py {} - ESP-AT Utility'.format(__version__), prog='at.py')

    subparsers = parser.add_subparsers(
    dest='operation',
    help='Run at.py {command} -h for additional help')

    parser_modify_bin = subparsers.add_parser(
        'modify_bin',
        help='Modify the parameter configuration of esp-at factory firmware (1MB/2MB/4MB/.. size) according to the parameter configuration')

    parser_generate_bin = subparsers.add_parser(
        'generate_bin',
        help='TODOs: ESP-AT will add this feature in v2.4.0.0+')

    subparsers.add_parser(
        'version',
        help='Print at.py version')

    """
    A typic format of esp-at parameter binary (4KB) is like the following:

    ┌─────────────────┬─────────┬────────┬────────┬────────┬───────┬───────┬──────────────────┬─────────────────┐
    │       2B        │    1B   │   1B   │   1B   │   1B   │  1B   │  1B   │        4B        │        4B       │
    ├─────────────────┼─────────┼────────┼────────┼────────┼───────┼───────┼──────────────────┼─────────────────┤
    │   magic code    │ version │  rsvd  │ tpower │ uart_x │ schan │ nchan │   country code   │    uart baud    │
    ├────────┬────────┼─────────┼────────┼────────┼────────┼───────┴───────┼──────────────────┴─────────────────┤
    │ tx pin │ rx pin │   cts   │  rts   │ txctrl │ rxctrl │     rsvd      │        platform (0-7)              │
    ├────────┴────────┴─────────┴────────┴────────┴────────┴───────────────┴────────────────────────────────────┤
    │                                              platform (8-23)                                              │
    ├──────────────────────────────────────────────────────────────────────┬────────────────────────────────────┤
    │                           platform (24-31)                           │        module name (0-7)           │
    ├──────────────────────────────────────────────────────────────────────┴────────────────────────────────────┤
    │                                            module name (8-23)                                             │
    ├──────────────────────────────────────────────────────────────────────┬────────────────────────────────────┤
    │                         module name (24-31)                          │         padded with 0xFF           │
    ├──────────────────────────────────────────────────────────────────────┴────────────────────────────────────┤
    │                                   padded with 0xFF bytes up to 4KB                                        │
    └───────────────────────────────────────────────────────────────────────────────────────────────────────────┘

    For the sake of generality, these parameters support to configure:
        <tpower>, <uart_x>, <schan>, <nchan>, <country code>, <uart baud>,
        <tx_pin>, <tx_pin>, <cts>, <rts>, <txctrl>, <rxctrl>, <platform>, <module name>

    For the sake of compatibility, these parameters do not support to configure:
        <magic code>, <version>, <rsvd>
    """

    parser_modify_bin.add_argument('--platform', '-pf',
        help='ESP chip series',
        type=lambda c: c[0:32],
        choices=['PLATFORM_ESP32', 'PLATFORM_ESP8266', 'PLATFORM_ESP32S2', 'PLATFORM_ESP32C3'])

    parser_modify_bin.add_argument('--module_name', '-mn',
        help='ESP module name',
        type=lambda c: c[0:32])

    parser_modify_bin.add_argument('--tx_power', '-tp',
        help='Initial RF Tx power of Wi-Fi, the unit is 0.25 dBm',
        type=int,
        choices=range(40, 85))

    parser_modify_bin.add_argument('--uart_num', '-un',
        help='Initial UART number for communication with host MCU, receive AT commands and response',
        type=int,
        choices=range(0, 3))

    parser_modify_bin.add_argument('--start_channel', '-sc',
        help='Initial Wi-Fi start channel',
        type=int,
        choices=range(1, 15))

    parser_modify_bin.add_argument('--channel_number', '-cn',
        help='Total Wi-Fi channel number',
        type=int,
        choices=range(1, 15))

    parser_modify_bin.add_argument('--country_code', '-cc',
        help='Initial Wi-Fi country code',
        type=lambda c: c[0:4])

    parser_modify_bin.add_argument('--baud', '-b',
        help='Initial UART baudrate of AT firmware, for communication with host MCU',
        type=arg_auto_int)

    parser_modify_bin.add_argument('--tx_pin', '-tx',
        help='GPIO pin of ESP-AT uart tx, ESP-AT uses this tx_pin to send data to host MCU',
        type=arg_auto_int)

    parser_modify_bin.add_argument('--rx_pin', '-rx',
        help='GPIO pin of ESP-AT uart rx, ESP-AT uses this rx_pin to receive data from host MCU',
        type=arg_auto_int)

    parser_modify_bin.add_argument('--cts_pin', '-cts',
        help='GPIO pin of ESP-AT uart cts, used for hardware flow control',
        type=arg_auto_int)

    parser_modify_bin.add_argument('--rts_pin', '-rts',
        help='GPIO pin of ESP-AT uart rts, used for hardware flow control',
        type=arg_auto_int)

    parser_modify_bin.add_argument('--tx_control_pin', '-txctrl',
        help='See the Figure 1-10b (ESP8266EX UART SWAP) in <ESP8266 Hardware Design Guidelines> for more details.',
        type=arg_auto_int)

    parser_modify_bin.add_argument('--rx_control_pin', '-rxctrl',
        help='See the Figure 1-10c (ESP8266EX UART SWAP) in <ESP8266 Hardware Design Guidelines> for more details.',
        type=arg_auto_int)

    parser_modify_bin.add_argument('--parameter_offset', '-os',
        help='Offset of parameter partition in AT firmware. If this parameter is set, the input file will be parsed directly according to the parameter instead of automatically matching the parameter partition header.',
        type=arg_auto_int)

    parser_modify_bin.add_argument('--input', '-in',
        help='Input filename of AT firmware or parameter partition',
        metavar='filename',
        type=str,
        required=True)

    parser_modify_bin.add_argument('--output', '-o',
        help='Output filename of AT firmware or parameter partition',
        metavar='filename',
        type=str,
        default='target.bin')

    for operation in subparsers.choices.keys():
        assert operation in globals(), "{} should be a module function".format(operation)

    args = parser.parse_args(argv)

    if args.operation is None:
        parser.print_help()
        sys.exit(1)

    operation_func = globals()[args.operation]

    try:
        operation_func(esp, args)
    finally:
        # do final cleanup
        pass

class FatalError(RuntimeError):
    """
    Wrapper class for runtime errors that aren't caused by internal bugs, but by
    ESP-AT responses or input content.
    """
    def __init__(self, message):
        RuntimeError.__init__(self, message)

    @staticmethod
    def WithResult(message, result):
        """
        Return a fatal error object that appends the hex values of
        'result' as a string formatted argument.
        """
        message += " (result was {})".format(hexify(result))
        return FatalError(message)

def _main():
    try:
        main()
    except FatalError as e:
        ESP_LOGE("A fatal error occurred: {}".format(e))
        sys.exit(2)
    except Exception as e:
        ESP_LOGE("A system error occurred: {}".format(e))

if __name__ == '__main__':
    _main()
