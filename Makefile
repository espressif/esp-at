#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp-at

export PROJECT_PATH := $(PWD)
export IDF_PATH ?= $(PWD)/esp-idf

EXTRA_CFLAGS += -DSDK_GIT=IDF_VER
include $(IDF_PATH)/make/project.mk

factory_bin:
	$(PYTHON) $(PROJECT_PATH)/tools/esp32_at_combine.py \
		--bin_directory $(PROJECT_PATH)/build \
		--flash_mode $(CONFIG_ESPTOOLPY_FLASHMODE) \
		--flash_size $(CONFIG_ESPTOOLPY_FLASHSIZE) \
		--flash_speed $(CONFIG_ESPTOOLPY_FLASHFREQ) \
		--parameter_file  $(PROJECT_PATH)/build/factory/factory_parameter.log