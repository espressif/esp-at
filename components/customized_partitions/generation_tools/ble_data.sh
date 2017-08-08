#!/bin/bash

PARTITION_NAME=$1
TARGET_FOLDER=$2
TOOL_PATH=$PROJECT_PATH/tools/BLEService.py

RAW_DATA_FOLDER=$PROJECT_PATH/components/customized_partitions/raw_data/ble_data

RAW_DATA_FILE=$(ls $RAW_DATA_FOLDER/*.csv | awk '{print $1}')

echo "generating ble_data.bin: python $TOOL_PATH -t $TARGET_FOLDER/$PARTITION_NAME.bin $RAW_DATA_FILE"

python $TOOL_PATH -t $TARGET_FOLDER/$PARTITION_NAME.bin $RAW_DATA_FILE
