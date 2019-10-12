COMPONENT_ADD_INCLUDEDIRS := port wolfssl

WOLFSSL_LIB_PATH := $(COMPONENT_PATH)/wolfssl/lib

ifdef CONFIG_ESP_DEBUG_WOLFSSL
    ifdef CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY
        # use debug library with WOLFSSL_SMALL_CERT_VERIFY enabled
        COMPONENT_ADD_LDFLAGS := $(WOLFSSL_LIB_PATH)/libwolfssl_small_cert_verify_debug.a
        COMPONENT_ADD_LINKER_DEPS := $(WOLFSSL_LIB_PATH)/libwolfssl_small_cert_verify_debug.a
    else
        # use debug library without WOLFSSL_SMALL_CERT_VERIFY enabled
        COMPONENT_ADD_LDFLAGS := $(WOLFSSL_LIB_PATH)/libwolfssl_debug.a
        COMPONENT_ADD_LINKER_DEPS := $(WOLFSSL_LIB_PATH)/libwolfssl_debug.a
    endif
else
    ifdef CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY
        # use library with WOLFSSL_SMALL_CERT_VERIFY enabled (without debug)
        COMPONENT_ADD_LDFLAGS := $(WOLFSSL_LIB_PATH)/libwolfssl_small_cert_verify.a
        COMPONENT_ADD_LINKER_DEPS := $(WOLFSSL_LIB_PATH)/libwolfssl_small_cert_verify.a
    else
        # use library without WOLFSSL_SMALL_CERT_VERIFY enabled (without debug)
        # This Library is USED BY DEFAULT (no wolfssl options enabled)
        COMPONENT_ADD_LDFLAGS := $(WOLFSSL_LIB_PATH)/libwolfssl.a
        COMPONENT_ADD_LINKER_DEPS := $(WOLFSSL_LIB_PATH)/libwolfssl.a
    endif
endif

CFLAGS +=-DWOLFSSL_USER_SETTINGS
