#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := http-at

export ESP_AT_PROJECT_PATH := $(PWD)/../..
export IDF_PATH ?= $(ESP_AT_PROJECT_PATH)/esp-idf

EXTRA_COMPONENT_DIRS := $(ESP_AT_PROJECT_PATH)/components

ESPTOOL_ALL_FLASH_ARGS += 0x280000 $(BUILD_DIR_BASE)/fatfs_image.img

EXTRA_CFLAGS += -DSDK_GIT=IDF_VER
include $(IDF_PATH)/make/project.mk

FATFS_IMAGE_COMPONENT_PATH := $(PWD)

MKFATFS_BIN="mkfatfs"

all_binaries: makefatfs

makefatfs: $(SDKCONFIG_MAKEFILE) mkfatfs
	@echo "Making fatfs image ..."
	@echo "$(ESPTOOLPY_WRITE_FLASH)"
	$(PWD)/components/mkfatfs/src/$(MKFATFS_BIN) -c $(FATFS_IMAGE_COMPONENT_PATH)/image -s 1048576 $(BUILD_DIR_BASE)/fatfs_image.img -d 2
