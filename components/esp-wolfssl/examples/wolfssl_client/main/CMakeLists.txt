idf_component_register(SRCS "wolfssl_client_demo.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES "server_root_cert.pem")

target_compile_definitions(${COMPONENT_LIB} PRIVATE WOLFSSL_USER_SETTINGS)
