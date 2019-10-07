COMPONENT_ADD_INCLUDEDIRS := port wolfssl

ifdef CONFIG_ESP_DEBUG_WOLFSSL
    ifdef CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY
        # use debug library with WOLFSSL_SMALL_CERT_VERIFY enabled
        COMPONENT_ADD_LDFLAGS := $(COMPONENT_PATH)/lib/libwolfssl_small_cert_verify_debug.a
        COMPONENT_ADD_LINKER_DEPS := $(COMPONENT_PATH)/lib/libwolfssl_small_cert_verify_debug.a
    else
        # use debug library without WOLFSSL_SMALL_CERT_VERIFY enabled
        COMPONENT_ADD_LDFLAGS := $(COMPONENT_PATH)/lib/libwolfssl_debug.a
        COMPONENT_ADD_LINKER_DEPS := $(COMPONENT_PATH)/lib/libwolfssl_debug.a
    endif
else
    ifdef CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY
        # use library with WOLFSSL_SMALL_CERT_VERIFY enabled (without debug)
        COMPONENT_ADD_LDFLAGS := $(COMPONENT_PATH)/lib/libwolfssl_small_cert_verify.a
        COMPONENT_ADD_LINKER_DEPS := $(COMPONENT_PATH)/lib/libwolfssl_small_cert_verify.a
    else
        # use library without WOLFSSL_SMALL_CERT_VERIFY enabled (without debug)
        # This Library is USED BY DEFAULT (no wolfssl options enabled)
        COMPONENT_ADD_LDFLAGS := $(COMPONENT_PATH)/lib/libwolfssl.a
        COMPONENT_ADD_LINKER_DEPS := $(COMPONENT_PATH)/lib/libwolfssl.a
    endif
endif

CFLAGS +=-DWOLFSSL_USER_SETTINGS
