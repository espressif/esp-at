#!/usr/bin/env bash
# -*- coding: utf-8 -*-
#
# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

RESULT=0
STARS='***************************************************'

find en -type f | cut -d/ -f2- | sort > file_list_en
find zh_CN -type f | cut -d/ -f2- | sort > file_list_zh_CN

# format is to display new or different filenames
DIFF_FORMAT="--unchanged-line-format= --old-line-format=[en]:%L --new-line-format=[zh_CN]:%L"

FOLDER_DIFFERENCES=$(diff $DIFF_FORMAT file_list_en file_list_zh_CN)
if ! [ -z "$FOLDER_DIFFERENCES" ]; then
    echo "$STARS"
    echo "Build failed due to the following differences in 'en' and 'zh_CN' folders:"
    echo "$FOLDER_DIFFERENCES"
    echo "$STARS"
    echo "Please synchronize contents of 'en' and 'zh_CN' folders to contain files with identical names"
    RESULT=1
fi

# remove temporary files
rm file_list_en file_list_zh_CN

exit $RESULT
