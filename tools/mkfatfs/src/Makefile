#CFLAGS		?= -std=gnu99 -Os -Wall
#CXXFLAGS	?= -std=gnu++11 -Os -Wall

IDF_INCLUDES = -I $(BUILD_DIR_BASE)/include

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

OBJ             := main.o \
		   fatfs/fatfs.o \
		   fatfs/ccsbcs.o \
		   fatfs/crc.o \
		   fatfs/FatPartition.o \
		   $(IDF_MODIFIED_DIR)/fatfs/src/ff.o \
		   $(IDF_MODIFIED_DIR)/fatfs/src/vfs_fat.o \
		   $(IDF_MODIFIED_DIR)/freertos/include/freertos/semphr.o \
		   $(IDF_MODIFIED_DIR)/newlib/include/sys/lock.o \
		   $(IDF_MODIFIED_DIR)/newlib/include/sys/idf_reent.o \
		   $(IDF_MODIFIED_DIR)/newlib/include/sys/errno.o \
		   $(IDF_MODIFIED_DIR)/spi_flash/partition.o \
		   $(IDF_MODIFIED_DIR)/vfs/vfs.o \
		   $(IDF_MODIFIED_DIR)/wear_levelling/wear_levelling.o \
		   $(IDF_ORIG_DIR)/fatfs/src/diskio.o \
		   $(IDF_ORIG_DIR)/fatfs/src/diskio_spiflash.o \
		   $(IDF_ORIG_DIR)/fatfs/src/option/syscall.o \
		   $(IDF_ORIG_DIR)/wear_levelling/crc32.o \
		   $(IDF_ORIG_DIR)/wear_levelling/WL_Flash.o \
		   $(IDF_ORIG_DIR)/wear_levelling/WL_Ext_Perf.o \
		   $(IDF_ORIG_DIR)/wear_levelling/WL_Ext_Safe.o \

				   
VERSION ?= $(shell git describe --always)

.PHONY: all clean

all: $(TARGET)

$(TARGET):
	@echo "Building mkfatfs ..."
	$(CXX) $(TARGET_CXXFLAGS) -c main.cpp -o main.o
	$(CC) $(TARGET_CFLAGS) -c fatfs/fatfs.c -o fatfs/fatfs.o
	$(CC) $(TARGET_CFLAGS) -c fatfs/ccsbcs.c -o fatfs/ccsbcs.o
	$(CXX) $(TARGET_CXXFLAGS) -c fatfs/crc.cpp -o fatfs/crc.o
	$(CXX) $(TARGET_CXXFLAGS) -c fatfs/FatPartition.cpp -o fatfs/FatPartition.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_MODIFIED_DIR)/fatfs/src/ff.c -o $(IDF_MODIFIED_DIR)/fatfs/src/ff.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_MODIFIED_DIR)/fatfs/src/vfs_fat.c -o $(IDF_MODIFIED_DIR)/fatfs/src/vfs_fat.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_MODIFIED_DIR)/freertos/include/freertos/semphr.c -o $(IDF_MODIFIED_DIR)/freertos/include/freertos/semphr.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_MODIFIED_DIR)/newlib/include/sys/lock.c -o $(IDF_MODIFIED_DIR)/newlib/include/sys/lock.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_MODIFIED_DIR)/newlib/include/sys/idf_reent.c -o $(IDF_MODIFIED_DIR)/newlib/include/sys/idf_reent.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_MODIFIED_DIR)/newlib/include/sys/errno.c -o $(IDF_MODIFIED_DIR)/newlib/include/sys/errno.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_MODIFIED_DIR)/spi_flash/partition.c -o $(IDF_MODIFIED_DIR)/spi_flash/partition.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_MODIFIED_DIR)/vfs/vfs.c -o $(IDF_MODIFIED_DIR)/vfs/vfs.o
	$(CXX) $(TARGET_CXXFLAGS) -c $(IDF_MODIFIED_DIR)/wear_levelling/wear_levelling.cpp -o $(IDF_MODIFIED_DIR)/wear_levelling/wear_levelling.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_ORIG_DIR)/fatfs/src/diskio.c -o $(IDF_ORIG_DIR)/fatfs/src/diskio.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_ORIG_DIR)/fatfs/src/diskio_spiflash.c -o $(IDF_ORIG_DIR)/fatfs/src/diskio_spiflash.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_ORIG_DIR)/fatfs/src/option/syscall.c -o $(IDF_ORIG_DIR)/fatfs/src/option/syscall.o
	$(CXX) $(TARGET_CXXFLAGS) -c $(IDF_ORIG_DIR)/wear_levelling/crc32.cpp -o $(IDF_ORIG_DIR)/wear_levelling/crc32.o
	$(CXX) $(TARGET_CXXFLAGS) -c $(IDF_ORIG_DIR)/wear_levelling/WL_Flash.cpp -o $(IDF_ORIG_DIR)/wear_levelling/WL_Flash.o
	$(CXX) $(TARGET_CXXFLAGS) -Wno-sign-compare -c $(IDF_ORIG_DIR)/wear_levelling/WL_Ext_Perf.cpp -o $(IDF_ORIG_DIR)/wear_levelling/WL_Ext_Perf.o
	$(CXX) $(TARGET_CXXFLAGS) -Wno-sign-compare -c $(IDF_ORIG_DIR)/wear_levelling/WL_Ext_Safe.cpp -o $(IDF_ORIG_DIR)/wear_levelling/WL_Ext_Safe.o
	$(CXX) $(TARGET_CFLAGS) -o $(TARGET) $(OBJ) $(TARGET_LDFLAGS)


	
clean:
	@rm -f *.o
	@rm -f fatfs/*.o
	@rm -f $(IDF_MODIFIED_DIR)/fatfs/src/*.o
	@rm -f $(IDF_MODIFIED_DIR)/freertos/include/freertos/*.o
	@rm -f $(IDF_MODIFIED_DIR)/newlib/include/sys/*.o
	@rm -f $(IDF_MODIFIED_DIR)/spi_flash/*.o
	@rm -f $(IDF_MODIFIED_DIR)/vfs/*.o
	@rm -f $(IDF_MODIFIED_DIR)/wear_levelling/*.o
	@rm -f $(IDF_ORIG_DIR)/fatfs/src/*.o
	@rm -f $(IDF_ORIG_DIR)/fatfs/src/option/*.o
	@rm -f $(IDF_ORIG_DIR)/wear_levelling/*.o
	@rm -f $(TARGET)
