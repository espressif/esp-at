#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

ifdef CONFIG_AT_BASE_ON_UART 
COMPONENT_SRCDIRS += interface/uart
endif

ifdef CONFIG_AT_BASE_ON_HSPI 
COMPONENT_SRCDIRS += interface/hspi
endif

ifdef CONFIG_AT_BASE_ON_SOCKET 
COMPONENT_SRCDIRS += interface/socket
endif

COMPONENT_ADD_INCLUDEDIRS := .


