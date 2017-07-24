#!/bin/bash

PARTITION_NAME=$1
TARGET_FOLDER=$2
TOOL_PATH=$PROJECT_PATH/tools/AtPKI.py

RAW_DATA_FOLDER=$PROJECT_PATH/components/customized_partitions/raw_data/server_key

RAW_DATA_FILE=$(ls $RAW_DATA_FOLDER/*.pem $RAW_DATA_FOLDER/*.der $RAW_DATA_FOLDER/*.key 2>/dev/null | awk '{print $1}')

echo "generating server_key.bin: python $TOOL_PATH generate_bin -b $TARGET_FOLDER/$PARTITION_NAME key $RAW_DATA_FILE"

python $TOOL_PATH generate_bin -b $TARGET_FOLDER/$PARTITION_NAME.bin key $RAW_DATA_FILE
