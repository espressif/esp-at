#!/bin/bash
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

PARTITION_NAME=$1
TARGET_FOLDER=$2
PKI_TYPE=$3
TOOL_PATH="$ESP_AT_PROJECT_PATH/tools/AtPKI.py"

RAW_DATA_FOLDER="$ESP_AT_PROJECT_PATH/components/customized_partitions/raw_data/$PARTITION_NAME"

if [ "$PKI_TYPE" == "cert" -o "$PKI_TYPE" == "ca" ]
then
    RAW_DATA_FILES=$(ls $RAW_DATA_FOLDER/*.pem $RAW_DATA_FOLDER/*.der $RAW_DATA_FOLDER/*.cer $RAW_DATA_FOLDER/*.crt 2>/dev/null | awk '{print $1}')
elif [[ "$PKI_TYPE" == "key" ]]
then
    RAW_DATA_FILES=$(ls $RAW_DATA_FOLDER/*.pem $RAW_DATA_FOLDER/*.der $RAW_DATA_FOLDER/*.key 2>/dev/null | awk '{print $1}')
fi

FILE_CONFIG=""
for FILE in $RAW_DATA_FILES;
do
    FILE_CONFIG="${FILE_CONFIG}${PKI_TYPE} $FILE "
done

if [ -z "$FILE_CONFIG" ]
then
    echo "$PARTITION_NAME: Warning: raw data file not found, generate empty file instead!"
    touch $TARGET_FOLDER/$PARTITION_NAME.bin
    exit 0
fi

echo "generating $PARTITION_NAME.bin: python $TOOL_PATH generate_bin -b $TARGET_FOLDER/$PARTITION_NAME $FILE_CONFIG"

python $TOOL_PATH generate_bin -b $TARGET_FOLDER/$PARTITION_NAME.bin $FILE_CONFIG
