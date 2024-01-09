#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import sys, os, re

if sys.version_info[0] == 2:
    reload(sys)
    sys.setdefaultencoding('utf-8')

def ESP_LOGI(x):
    print('\033[32m{}\033[0m'.format(x))

def ESP_LOGE(x):
    print('\033[31m{}\033[0m'.format(x))

# allowed characters, include some chinese characters, symbol, and punctuation
at_allowed_chars_list = ['中文', '®', '℃', '…', '✅', '❌', '√', '×', '├', '└', '│', '–', '—']
at_not_allowed_chars_list = re.compile(b'[^\x0a\x0d\x20-\x7e]')
at_file_white_list = ['index_of_abbreviations.rst']

def at_get_file_list(doc_path, subdir_file_list):
    if os.path.isdir(doc_path):
        file_list = os.listdir(doc_path)
    else:
        subdir_file_list.append(doc_path)
        return subdir_file_list
    for file in file_list:
        cur_path = os.path.join(doc_path, file)
        if os.path.isdir(cur_path):
            at_get_file_list(cur_path, subdir_file_list)
        else:
            subdir_file_list.append(cur_path)
    return subdir_file_list

def at_data_is_allowed_chars(match_info, data):
    to_check_idx = match_info.span()
    s_last_idx = -2
    for cur_idx in to_check_idx:
        if (cur_idx == s_last_idx + 1):
            s_last_idx = cur_idx
            continue
        else:
            chars_is_valid = 0
            for chars in at_allowed_chars_list:
                cur_allowed_data = chars.encode()
                to_check_data_tail_idx = cur_idx + len(chars.encode())
                to_check_data = data[cur_idx : to_check_data_tail_idx]
                if cur_allowed_data == to_check_data:
                    chars_is_valid = 1
            if chars_is_valid == 1:
                return True
            s_last_idx = cur_idx
    return False

def at_check_doc_chars_validity(doc_name):
    with open(doc_name, 'rb') as fp:
        for (lineno, data) in enumerate(fp):
            match_info = re.search(at_not_allowed_chars_list, data)
            if match_info:
                if not at_data_is_allowed_chars(match_info, data):
                    ESP_LOGE('Error: illegal character detected at {}:{}'.format(doc_name, lineno + 1))
                    print('raw data ----> {}\r\n'.format(data))
                    print('Allowed chars:')
                    for x in at_allowed_chars_list:
                        print(x, '---->', x.encode())
                    return False
            pass
    return True

def _main():
    if len(sys.argv) == 2:
        dst_path = os.path.abspath(sys.argv[1])
    else:
        dst_path = os.path.abspath('.') + '/en'
    at_en_doc_file_list = at_get_file_list(dst_path, [])
    for current_file in at_en_doc_file_list:
        for file_basename in at_file_white_list:
            if os.path.basename(current_file) == file_basename:
                continue
            else:
                if at_check_doc_chars_validity(current_file) == False:
                    sys.exit(-1)
    ESP_LOGI('Document characters check passed! ({})'.format(dst_path))

if __name__ == '__main__':
    _main()
