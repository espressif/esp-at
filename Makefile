#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp-at

export PROJECT_PATH := $(PWD)
export IDF_PATH ?= $(PWD)/esp-idf

EXTRA_CFLAGS += -DSDK_GIT=IDF_VER
include $(IDF_PATH)/make/project.mk
