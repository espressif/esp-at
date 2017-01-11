#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp-at
EXTRA_CFLAGS += -DSDK_GIT=IDF_VER
include $(IDF_PATH)/make/project.mk

