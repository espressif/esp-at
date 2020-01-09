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

ESP_AT_IMAGE_SIZE_STR=$3

if [ ${ESP_AT_IMAGE_SIZE_STR: -1} == K ]
then 
	ESP_AT_IMAGE_SIZE_STR=${ESP_AT_IMAGE_SIZE_STR%?}
	let ESP_AT_IMAGE_SIZE=$[ESP_AT_IMAGE_SIZE_STR]*1024
else
	let ESP_AT_IMAGE_SIZE=$[ESP_AT_IMAGE_SIZE_STR]
fi

make mkfatfs

"$ESP_AT_PROJECT_PATH"/tools/mkfatfs/src/mkfatfs -c "$ESP_AT_IMAGE_DIR" -s $ESP_AT_IMAGE_SIZE $2/$1.bin