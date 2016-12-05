#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp-at
EXTRA_CFLAGS += -DSDK_GIT=\"$(shell cd $(IDF_PATH);git rev-list HEAD -n 1 | cut -c 1-8)\"
include $(IDF_PATH)/make/project.mk

