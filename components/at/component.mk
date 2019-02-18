#
# Component Makefile
#

LIBS := $(shell echo $(patsubst PLATFORM_%,%_at_core,$(ESP_AT_PROJECT_PLATFORM)) | tr A-Z a-z)

COMPONENT_ADD_LDFLAGS := -L $(COMPONENT_PATH)/lib \
                           $(addprefix -l,$(LIBS))

COMPONENT_ADD_LINKER_DEPS := $(patsubst %,$(COMPONENT_PATH)/lib/lib%.a,$(LIBS))

ALL_LIB_FILES := $(patsubst %,$(COMPONENT_PATH)/lib/lib%.a,$(LIBS))
$(COMPONENT_LIBRARY): $(ALL_LIB_FILES)
