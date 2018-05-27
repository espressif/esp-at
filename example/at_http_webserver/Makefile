#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := http-at

export ESP_AT_PROJECT_PATH := $(PWD)/../..
export IDF_PATH ?= $(ESP_AT_PROJECT_PATH)/esp-idf

include $(PWD)/sdkconfig

EXTRA_COMPONENT_DIRS := $(ESP_AT_PROJECT_PATH)/components

#AT_FATFS_IMAGE_OFFSET := `cat partitions_at.csv |grep \'^[^#]\' |grep fat|awk -F, '{print $$4}' `

AT_FATFS_IMAGE_OFFSET := `cat $(CONFIG_PARTITION_TABLE_FILENAME)|grep -v '\#'|grep fat|awk -F, '{print $$4}' `

AT_FATFS_IMAGE_SIZE := `cat $(CONFIG_PARTITION_TABLE_FILENAME) |grep -v '\#'|grep 'fat'|awk -F, '{print $$5}' `

ESPTOOL_ALL_FLASH_ARGS += $(AT_FATFS_IMAGE_OFFSET) $(BUILD_DIR_BASE)/fatfs_image.img

EXTRA_CFLAGS += -DSDK_GIT=IDF_VER

include $(IDF_PATH)/make/project.mk

FATFS_IMAGE_COMPONENT_PATH := $(PWD)

MKFATFS_BIN="mkfatfs"

all_binaries: makefatfs

makefatfs: $(SDKCONFIG_MAKEFILE) mkfatfs
	@echo "Making fatfs image ..."
	@echo "$(ESPTOOLPY_WRITE_FLASH)"
	$(PWD)/components/mkfatfs/src/$(MKFATFS_BIN) -c $(FATFS_IMAGE_COMPONENT_PATH)/image -s $(AT_FATFS_IMAGE_SIZE) $(BUILD_DIR_BASE)/fatfs_image.img -d 2

