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

import os
import sys
import re
import argparse

import yaml
import xlrd
import csv
import json

ERROR_LOG = ".tmp_error_log"


# implementation of source file parser

def input_format(func):
    def _handle_args(data, **kwargs):
        if isinstance(data, str):
            value = data.encode("utf-8")
        elif isinstance(data, float):
            value = str(int(data))
        else:
            value = str(data)
        return func(value, **kwargs)
    return _handle_args


class ComponentParser(object):

    def __init__(self, config):
        self.parser_table = config

    @staticmethod
    @input_format
    def hex(data, out_format="int"):
        """
        parse and convert to required hex format
        :param data: hex or 0xhex
        :param out_format: output format
        :return: hex without prefix and is upper case
        """
        value = int(data, base=16)
        if out_format == "int":
            return value
        elif out_format == "string":
            s = "%X" % value
            if len(s) % 2 == 1:
                s = "0" + s
            return s
        else:
            return None

    @staticmethod
    @input_format
    def int(data, out_format="int"):
        """
        parse and convert to required int format
        :param data: base 10 int or 0x + hex or 0b + binary
        :param out_format: output format
        :return: base 10 int
        """
        if data[:2] == "0x":
            ret = int(data, base=16)
        elif data[:2] == "0b":
            ret = int(data, base=2)
        else:
            ret = int(data, base=10)
        if out_format == "int":
            return ret
        elif out_format == "string":
            return str(ret)
        else:
            return None

    @staticmethod
    @input_format
    def string(data, out_format="string"):
        if out_format == "string":
            return data
        else:
            return None

    @staticmethod
    @input_format
    def hex_string(data, out_format="string"):
        if data[:2] == "0x":
            data = data[2:]
        tmp = "".join(chr(int(data[2*x:2*x+2], base=16)) for x in range(len(data) // 2 + len(data) % 2))
        if out_format == "string":
            return "".join("%02X" % ord(x) for x in tmp)
        else:
            return None

    def parse(self, cp_pair, error_handler):
        config = self.parser_table[cp_pair["key"]]
        parser = self.__getattribute__(config["type"])
        if cp_pair["value"] is None or cp_pair["value"] == "":
            ret = config["default"]
        else:
            if "outFormat" in config:
                ret = parser(cp_pair["value"], out_format=config["outFormat"])
            else:
                ret = parser(cp_pair["value"])
        if ret is None:
            error_handler("failed to parse")
            error_handler("[key] " + cp_pair["key"])
            error_handler("[value] " + str(ret))
        # do verify parsed data with RegEx pattern
        if "requiredPattern" in config:
            pattern = re.compile(config["requiredPattern"])
            if pattern.search(str(ret)) is False:
                error_handler("verify data fail")
                error_handler("[key] " + cp_pair["key"])
                error_handler("[value] " + str(ret))
                error_handler("[required pattern] " + config["requiredPattern"])
        return ret


class BaseParser(object):
    def __init__(self, file_name, config, error_log_file):
        """
        init parser
        :param file_name: file to parse
        :param config: parser config
        :param error_log_file: error log file. if parse fail detects then write error info to this file.
        """
        self.file_name = file_name
        self.config = config
        self.error_log_file = error_log_file
        self.cp_parser = ComponentParser(self.config)

    def log_error(self, msg):
        with open(self.error_log_file, "ab+") as f:
            f.write(msg + "\r\n")

    def load(self):
        """
        load from file and return a dict. subclass of BaseParser should overwrite this method.
        :return: a list of loaded values. each value is a dict with all keys in config and its value.
        """
        return dict()

    def parse_data(self, input_dict):
        """ parse values """
        out_dict = dict()
        for d in input_dict:
            if sys.version_info.major == 2:
                if isinstance(d, str):
                    # convert key to utf-8 as well
                    d = d.encode("utf-8")
            try:
                out_dict[d] = self.cp_parser.parse({"key": d, "value": input_dict[d]}, self.log_error)
            except Exception as e:
                self.log_error("failed to parse: {key: %s, value: %s}" % (d, input_dict[d]))
                self.log_error("exception is %s" % e)
        return out_dict

    def parse(self):
        """ parse source file according to config """
        intermediate_data = self.load()
        return {"Service": [self.parse_data(x) for x in intermediate_data]}


class ExcelParser(BaseParser):
    MAX_PARSE_LINE = 1024

    def load(self):
        wb = xlrd.open_workbook(self.file_name)

        # only read the first sheet
        try:
            sheet = wb.sheet_by_index(0)
            title_row = sheet.row_values(0)
        except IndexError:
            self.log_error("failed to get sheet or title row")
            return

        # compute missing keys
        missing_keys = []
        for key in self.config:
            if key not in title_row:
                missing_keys.append(key)

        out_data = []
        for n in range(self.MAX_PARSE_LINE):
            try:
                row_value = sheet.row_values(n+1)
                if set(row_value) == {""}:
                    # stopped by empty row
                    break
                # need to set None for empty value ""
                data = {t: v if v != "" else None for t in title_row for v in row_value}
                data.update(dict.fromkeys(missing_keys, None))
                out_data.append(data)
            except IndexError:
                # parse finished
                break
        return out_data


class CSVParser(BaseParser):

    def load(self):
        with open(self.file_name, "r") as f:
            reader = csv.DictReader(f)
            tmp_data = [r for r in reader]

        if len(tmp_data) == 0:
            self.log_error("empty CSV file")
            return

        # compute missing keys
        missing_keys = []
        for key in self.config:
            if key not in tmp_data[0]:
                missing_keys.append(key)

        out_data = []
        for data in tmp_data:
            if set(data.values()) == {""}:
                # stopped by empty row
                break
            data.update(dict.fromkeys(missing_keys, None))
            out_data.append(data)

        return out_data


# implementation of target file dumper

class BaseDumper(object):
    def __init__(self, target_file_name, config_file, error_log_file):
        """
        init dumper
        :param target_file_name: target file to dump
        :param config_file: converter config file
        :param error_log_file: error log file. if parse fail detects then write error info to this file.
        """
        self.target_file_name = target_file_name
        self.config_file = config_file
        self.error_log_file = error_log_file

    def log_error(self, msg):
        with open(self.error_log_file, "ab+") as f:
            f.write(msg + "\r\n")

    def convert_data(self, data):
        """
        convert raw data to target format. subclass MUST overwrite this method.
        :param data: a list of dict that returned from parser
        :return: converted data
        """
        return ""

    def post_process(self, data):
        """
        do post process like adding header according to config file
        :param data: data returned by convert_data
        :return: processed data
        """
        config_file_name = os.path.split(self.config_file)[1]
        if config_file_name == "ATBleService.yml":
            # append 2 bytes LE data len + 2 bytes magic code 0x27 0x95
            data = chr(len(data) & 0xFF) + chr(len(data) >> 8) \
                   + chr(0x27) + chr(0x95) \
                   + data
        else:
            # default not appending anything
            pass
        return data

    def dump(self, data):
        """
        dump data to target file with required format
        :param data: data returned from parser
        :return: None
        """
        data = self.convert_data(data)
        data = self.post_process(data)
        dir_name = os.path.dirname(self.target_file_name)
        if dir_name != "" and os.path.exists(dir_name) is False:
            os.makedirs(os.path.dirname(self.target_file_name))
        with open(self.target_file_name, "w+") as f:
            f.write(data)


class JsonDumper(BaseDumper):
    def convert_data(self, data):
        return json.dumps(data)


# command line and main function

PARSERS = {
    "xls": ExcelParser,
    "xlsx": ExcelParser,
    "csv": CSVParser,
}

DUMPERS = {
    "json": JsonDumper,
}


def parse_args(script_path):
    parser = argparse.ArgumentParser()
    parser.add_argument("source_file",
                        help="source file to be parsed")
    parser.add_argument("-f", "--dump_format", default="json",
                        help="dump format")
    parser.add_argument("-t", "--target_file", default=None,
                        help="target file")
    parser.add_argument("-c", "--config_file", default=None,
                        help="converter config file")

    args = parser.parse_args()

    if args.config_file is None:
        config_file = os.path.join(os.path.dirname(script_path), "Configs/ATBleService.yml")
    else:
        config_file = args.config_file

    if args.target_file is None:
        target_file = os.path.join(os.path.dirname(args.source_file),
                                   os.path.splitext(os.path.split(config_file)[1])[0] + ".bin")
    else:
        target_file = args.target_file

    return args.source_file, args.dump_format, target_file, config_file


def main():
    source_file, dump_format, target_file, config_file = parse_args(sys.argv[0])
    with open(config_file, "rb") as cf:
        config = yaml.load(cf, Loader=yaml.Loader)

    def get_ext(f):
        return os.path.splitext(f)[1][1:]
    if get_ext(source_file) not in PARSERS:
        print("don't support source file format")
        print(("only support the following format: %s" % list(PARSERS.keys())))
        sys.exit(2)
    if dump_format not in DUMPERS:
        print("dump format not supported")
        print(("only support the following format: %s" % list(DUMPERS.keys())))
        sys.exit(2)

    try:
        # try to remove error log
        os.remove(ERROR_LOG)
    except OSError:
        pass

    error = False
    try:
        parser = PARSERS[get_ext(source_file)](source_file, config, ERROR_LOG)
        data = parser.parse()
        dumper = DUMPERS[dump_format](target_file, config_file, ERROR_LOG)
        dumper.dump(data)
    except Exception as e:
        print(e)
        error = True

    if os.path.exists(ERROR_LOG):
        with open(ERROR_LOG, "rb") as error_log_file:
            error_info = error_log_file.read()
            if error_info != "":
                print(error_info)
                error = True
        os.remove(ERROR_LOG)

    sys.exit(1 if error else 0)

if __name__ == '__main__':
    main()
