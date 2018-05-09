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
TOOL_PATH=$ESP_AT_PROJECT_PATH/tools/BLEService.py

RAW_DATA_FOLDER=$ESP_AT_PROJECT_PATH/components/customized_partitions/raw_data/ble_data

RAW_DATA_FILE=$(ls $RAW_DATA_FOLDER/*.csv | awk '{print $1}')

echo "generating ble_data.bin: python $TOOL_PATH -t $TARGET_FOLDER/$PARTITION_NAME.bin $RAW_DATA_FILE"

python $TOOL_PATH -t $TARGET_FOLDER/$PARTITION_NAME.bin $RAW_DATA_FILE
