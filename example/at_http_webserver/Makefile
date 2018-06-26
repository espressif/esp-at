#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp-http-at

export ESP_AT_PROJECT_PATH := $(PWD)/../..
export IDF_PATH ?= $(ESP_AT_PROJECT_PATH)/esp-idf

export ESP_AT_IMAGE_DIR := $(PWD)/components/fs_image
EXTRA_COMPONENT_DIRS := $(ESP_AT_PROJECT_PATH)/tools/mkfatfs $(ESP_AT_PROJECT_PATH)/components

EXTRA_CFLAGS += -DSDK_GIT=IDF_VER

include $(IDF_PATH)/make/project.mk

factory_bin:
	$(PYTHON) $(ESP_AT_PROJECT_PATH)/tools/esp32_at_combine.py \
		--bin_directory $(ESP_AT_PROJECT_PATH)/build \
		--flash_mode $(CONFIG_ESPTOOLPY_FLASHMODE) \
		--flash_size $(CONFIG_ESPTOOLPY_FLASHSIZE) \
		--flash_speed $(CONFIG_ESPTOOLPY_FLASHFREQ)

