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

# allowed characters, include some chinese characters, symbol, and punctuation in en document
at_allowed_chars_list = ['中文', '®', '℃', '…', '✅', '❌', '√', '×', '├', '└', '│', '–', '—']
at_file_white_list = ['index_of_abbreviations.rst']
at_not_allowed_chars_list = re.compile(b'[^\x0a\x0d\x20-\x7e]')

# fullwidth space, letters, digits, punctuations that are not allowed in cn document
at_fullwidth_space = r'　'
at_fullwidth_letters = r'[Ａ-Ｚａ-ｚ]'
at_fullwidth_digits = r'[０-９]'
at_fullwidth_punctuations = r'[＠＃＄％＾＆＊－＋＝〈〉「」『』【】＼｜＇＂＜．＞／]'
at_fullwidth_file_white_list = ['index_of_abbreviations.rst']

def at_get_rst_list(path):
    rst_files = []
    for root, _, files in os.walk(path):
        for file in files:
            if file.endswith('.rst'):
                rst_files.append(os.path.abspath(os.path.join(root, file)))
    return rst_files

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
        for (lineno, data) in enumerate(fp, start=1):
            match_info = re.search(at_not_allowed_chars_list, data)
            if match_info:
                if not at_data_is_allowed_chars(match_info, data):
                    ESP_LOGE('Error: illegal character detected at {}:{}'.format(doc_name, lineno))
                    print(f'Line {lineno}: {data.strip()}')
                    print('\r\nBut allowed chars:')
                    for x in at_allowed_chars_list:
                        print(x, '---->', x.encode())
                    return False
            pass
    return True

def at_check_en_format():
    if len(sys.argv) == 2:
        dst_path = os.path.abspath(sys.argv[1]) + '/en'
    else:
        dst_path = os.path.abspath('.') + '/en'
    at_en_doc_file_list = at_get_rst_list(dst_path)
    for current_file in at_en_doc_file_list:
        for file_basename in at_file_white_list:
            if os.path.basename(current_file) == file_basename:
                continue
            else:
                if at_check_doc_chars_validity(current_file) == False:
                    return False

def at_check_fullwidth_chars(file_path):
    pattern = re.compile(f'{at_fullwidth_space}|{at_fullwidth_letters}|{at_fullwidth_digits}|{at_fullwidth_punctuations}')
    with open(file_path, 'r', encoding='utf-8') as file:
        for line_number, line in enumerate(file, start=1):
            matches = pattern.findall(line)
            if matches:
                ESP_LOGE('Error: illegal fullwidth character detected at {}:{}'.format(file_path, line_number))
                print(f'Line {line_number}: {line.strip()}')
                print(f"Full-width characters found: {', '.join(matches)}")
                return False
    return True

def at_check_cn_format():
    if len(sys.argv) == 2:
        dst_path = os.path.abspath(sys.argv[1]) + '/zh_CN'
    else:
        dst_path = os.path.abspath('.') + '/zh_CN'
    at_cn_doc_file_list = at_get_rst_list(dst_path)
    for current_file in at_cn_doc_file_list:
        if not at_fullwidth_file_white_list:
            for file_basename in at_fullwidth_file_white_list:
                if os.path.basename(current_file) == file_basename:
                    continue
                else:
                    if at_check_fullwidth_chars(current_file) == False:
                        return False
        else:
            if at_check_fullwidth_chars(current_file) == False:
                return False

def _main():
    if at_check_en_format() == False:
        sys.exit(-1)

    if at_check_cn_format() == False:
        sys.exit(-1)

    ESP_LOGI('Document characters check passed!')

if __name__ == '__main__':
    _main()
