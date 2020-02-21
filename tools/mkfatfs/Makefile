#CFLAGS		?= -std=gnu99 -Os -Wall
#CXXFLAGS	?= -std=gnu++11 -Os -Wall
# BUILD_DIR_BASE ?= $(ESP_AT_PROJECT_PATH)/build
IDF_INCLUDES = -I $(BUILD_DIR_BASE)/include

BIN_DIR = $(BUILD_DIR_BASE)/mkfatfs

IDF_MODIFIED_DIR = idf/modified
IDF_INCLUDES += -I $(IDF_MODIFIED_DIR)
IDF_INCLUDES += -I $(IDF_MODIFIED_DIR)/driver/include
IDF_INCLUDES += -I $(IDF_MODIFIED_DIR)/fatfs/src
IDF_INCLUDES += -I $(IDF_MODIFIED_DIR)/freertos/include
IDF_INCLUDES += -I $(IDF_MODIFIED_DIR)/log/include
IDF_INCLUDES += -I $(IDF_MODIFIED_DIR)/newlib/include
IDF_INCLUDES += -I $(IDF_MODIFIED_DIR)/vfs/include
IDF_INCLUDES += -I $(IDF_MODIFIED_DIR)/vfs/include/sys

IDF_ORIG_DIR = idf/orig
IDF_INCLUDES += -I $(IDF_ORIG_DIR)/driver/include
IDF_INCLUDES += -I $(IDF_ORIG_DIR)/driver/include/driver
IDF_INCLUDES += -I $(IDF_ORIG_DIR)/esp32/include
IDF_INCLUDES += -I $(IDF_ORIG_DIR)/fatfs/src
IDF_INCLUDES += -I $(IDF_ORIG_DIR)/sdmmc/include
IDF_INCLUDES += -I $(IDF_ORIG_DIR)/spi_flash/include
IDF_INCLUDES += -I $(IDF_ORIG_DIR)/wear_levelling/include
IDF_INCLUDES += -I $(IDF_ORIG_DIR)/wear_levelling/private_include

ifdef OS
ifeq ($(OS),Windows_NT)
  OS_IS_WIN = 1
endif
endif

ifdef OS_IS_WIN
	TARGET_OS := WINDOWS
	DIST_SUFFIX := windows
	ARCHIVE_CMD := 7z a
	ARCHIVE_EXTENSION := zip
	TARGET := mkfatfs.exe
	CC=gcc.exe
	CXX=g++.exe
	#MVA TARGET_CFLAGS := -mno-ms-bitfields
	CPATH := $(COMPONENT_INCLUDES)
	#TARGET_CFLAGS := -mno-ms-bitfields -std=gnu99 -Os -Wall -I $(COMPONENT_INCLUDES) -Itclap -Ifatfs -I. -D$(TARGET_OS)
	TARGET_CFLAGS := -mno-ms-bitfields -std=gnu99 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) $(IDF_INCLUDES)
	TARGET_CXXFLAGS	:= -std=gnu++11 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) $(IDF_INCLUDES)
	TARGET_LDFLAGS := -Wl,-static -static-libgcc

else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		TARGET_OS := LINUX
		UNAME_P := $(shell uname -p)
		ifeq ($(UNAME_P),x86_64)
			DIST_SUFFIX := linux64
		endif
		ifneq ($(filter %86,$(UNAME_P)),)
			DIST_SUFFIX := linux32
		endif
		CC=gcc
		CXX=g++
		TARGET_CFLAGS   = -std=gnu99 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) -DVERSION=\"$(VERSION)\" -D__NO_INLINE__  $(IDF_INCLUDES)
		TARGET_CXXFLAGS = -std=gnu++11 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) -DVERSION=\"$(VERSION)\" -D__NO_INLINE__  $(IDF_INCLUDES)
		TARGET_LDFLAGS  =
	endif
	ifeq ($(UNAME_S),Darwin)
		TARGET_OS := OSX
		DIST_SUFFIX := osx
		CC=clang
		CXX=clang++
		TARGET_CFLAGS   = -std=gnu99 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) -DVERSION=\"$(VERSION)\" -D__NO_INLINE__ -mmacosx-version-min=10.7 -arch x86_64 $(IDF_INCLUDES)
		TARGET_CXXFLAGS = -std=gnu++11 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) -DVERSION=\"$(VERSION)\" -D__NO_INLINE__ -mmacosx-version-min=10.7 -arch x86_64 -stdlib=libc++ $(IDF_INCLUDES)
		TARGET_LDFLAGS  = -arch x86_64 -stdlib=libc++
	endif

	ARCHIVE_CMD := tar czf
	ARCHIVE_EXTENSION := tar.gz
	TARGET := mkfatfs
endif

				   
VERSION ?= $(shell git describe --always)

C_SRCS = $(shell find . -name "*.c")
CPP_SRCS = $(shell find . -name "*.cpp")

C_OBJS = $(patsubst %.c, %.o, $(C_SRCS))
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SRCS))

.PHONY: all clean

all: $(TARGET)

%.o : %.c
	@mkdir -p $(dir $(BIN_DIR)/$@)
	$(CC) $(TARGET_CFLAGS) -c $< -o $(BIN_DIR)/$@

%.o : %.cpp
	@mkdir -p $(dir $(BIN_DIR)/$@)
	$(CXX) $(TARGET_CXXFLAGS) -c $< -o $(BIN_DIR)/$@

$(TARGET): $(C_OBJS) $(CPP_OBJS)
	@echo "Building mkfatfs ..."
	$(CXX) $(TARGET_CFLAGS) -o $(BIN_DIR)/$(TARGET) $(addprefix $(BIN_DIR)/,$(C_OBJS) $(CPP_OBJS)) $(TARGET_LDFLAGS)
	
clean:
	@rm -rf $(BIN_DIR)/*
