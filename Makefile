#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp-at
export ESP_AT_PROJECT_PLATFORM ?= PLATFORM_ESP32
export ESP_AT_MODULE_NAME ?= WROOM-32

export ESP_AT_PROJECT_PATH := $(PWD)

export ESP_AT_IMAGE_DIR ?= $(ESP_AT_PROJECT_PATH)/components/fs_image
EXTRA_COMPONENT_DIRS := $(ESP_AT_PROJECT_PATH)/tools/mkfatfs

export IDF_PATH ?= $(ESP_AT_PROJECT_PATH)/esp-idf

export SILENCE ?=
ifeq ($(SILENCE), 1)
EXTRA_CFLAGS += -DNDEBUG
endif

ESP_AT_PROJECT_COMMIT_ID := $(shell git rev-parse --short HEAD)
EXTRA_CFLAGS += -DESP_AT_PROJECT_COMMIT_ID=\"$(ESP_AT_PROJECT_COMMIT_ID)\"

ESP_AT_MODULE_CONFIG_DIR ?= module_config/module_$(shell echo $(ESP_AT_MODULE_NAME) | tr A-Z a-z)

ifeq (,$(wildcard $(ESP_AT_MODULE_CONFIG_DIR))) ## if there is no module config, we use platform default config
CONFIG_DIR := module_config/$(shell echo $(patsubst PLATFORM_%,module_%_default,$(ESP_AT_PROJECT_PLATFORM)) | tr A-Z a-z)
$(warning There is no $(ESP_AT_MODULE_CONFIG_DIR),Using $(CONFIG_DIR))
ESP_AT_MODULE_CONFIG_DIR := $(CONFIG_DIR)
endif

ifeq (,$(wildcard $(ESP_AT_MODULE_CONFIG_DIR))) ## if there is no module config, we use platform default config
$(error There is no $(ESP_AT_MODULE_CONFIG_DIR))
endif

esp_at_idf_branch := $(strip $(subst branch:,,$(shell sed -n /branch:/p $(ESP_AT_MODULE_CONFIG_DIR)/IDF_VERSION)))
esp_at_idf_commit := $(strip $(subst commit:,,$(shell sed -n /commit:/p $(ESP_AT_MODULE_CONFIG_DIR)/IDF_VERSION)))
esp_at_idf_repository := $(strip $(subst repository:,,$(shell sed -n /repository:/p $(ESP_AT_MODULE_CONFIG_DIR)/IDF_VERSION)))

ifeq (,$(wildcard $(IDF_PATH)))
$(info $(shell git clone -b $(esp_at_idf_branch) $(esp_at_idf_repository) $(IDF_PATH); \
			cd $(IDF_PATH); \
			git checkout $(esp_at_idf_commit); \
			git submodule update --init --recursive \
		))
else
idf_version := $(strip $(shell cd $(IDF_PATH);git rev-parse HEAD))
ifneq ($(esp_at_idf_commit), $(idf_version))
$(warning ======================================================)
$(warning esp-idf commit is incorrect, please ensure repository url is for $(ESP_AT_MODULE_NAME))
$(warning and I will checkout $(esp_at_idf_commit))
$(warning ======================================================)
$(info $(shell cd $(IDF_PATH); \
			git checkout $(esp_at_idf_branch); \
			git pull; \
			git checkout $(esp_at_idf_commit); \
			git submodule update --init --recursive \
		))
endif
endif

ifeq ($(SILENCE), 1)
SDKCONFIG_DEFAULTS := $(ESP_AT_MODULE_CONFIG_DIR)/sdkconfig_silence.defaults
else
SDKCONFIG_DEFAULTS := $(ESP_AT_MODULE_CONFIG_DIR)/sdkconfig.defaults
endif

export PROJECT_VER = "ESP-AT"

ifeq ("$(filter 3.81 3.82,$(MAKE_VERSION))","") ## IDF just support 3.81,3.82 or 4.x newer
include $(IDF_PATH)/make/project.mk
else
ifeq (,$(wildcard $(IDF_PATH)))
ifeq (,$(MAKECMDGOALS))
all:
	@make $(MAKECMDGOALS)
else
$(word 1,$(MAKECMDGOALS)):
	@make $(MAKECMDGOALS)
endif
else
include $(IDF_PATH)/make/project.mk
endif
endif

factory_bin:
	$(PYTHON) $(ESP_AT_PROJECT_PATH)/tools/esp_at_factory_bin_combine.py \
		--module_name $(ESP_AT_MODULE_NAME) \
		--bin_directory $(ESP_AT_PROJECT_PATH)/build \
		--flash_mode $(CONFIG_ESPTOOLPY_FLASHMODE) \
		--flash_size $(CONFIG_ESPTOOLPY_FLASHSIZE) \
		--flash_speed $(CONFIG_ESPTOOLPY_FLASHFREQ) \
		--parameter_file $(PROJECT_PATH)/build/factory/factory_parameter.log \
		--download_config $(PROJECT_PATH)/build/download.config