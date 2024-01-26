
# Tell the linker to reference the following symbol during the linking phase,
# to prevent the symbol from being optimized away.

if (CONFIG_AT_BASE_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_base_cmd_regist")
endif()

if (CONFIG_AT_WIFI_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_wifi_cmd_regist")
endif()

if (CONFIG_AT_SMARTCONFIG_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_smartconfig_cmd_regist")
endif()

if (CONFIG_AT_WPS_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_wps_cmd_regist")
endif()

if (CONFIG_AT_EAP_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_eap_cmd_regist")
endif()

if (CONFIG_AT_MDNS_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_mdns_cmd_regist")
endif()

if (CONFIG_AT_NET_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_net_cmd_regist")
endif()

if (CONFIG_AT_WEB_SERVER_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_web_server_cmd_regist")
endif()

if (CONFIG_AT_PING_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_ping_cmd_regist")
endif()

if (CONFIG_AT_OTA_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_ota_cmd_regist")
endif()

if (CONFIG_AT_MQTT_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_mqtt_cmd_regist")
endif()

if (CONFIG_AT_HTTP_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_http_cmd_regist")
endif()

if (CONFIG_AT_WS_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_ws_cmd_regist")
endif()

if (CONFIG_AT_BLE_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_ble_cmd_regist")
endif()

if (CONFIG_AT_BLE_HID_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_ble_hid_cmd_regist")
endif()

if (CONFIG_AT_BLUFI_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_blufi_cmd_regist")
endif()

if (CONFIG_AT_BT_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_bt_cmd_regist")
endif()

if (CONFIG_AT_BT_SPP_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_bt_spp_cmd_regist")
endif()

if (CONFIG_AT_BT_A2DP_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_bt_a2dp_cmd_regist")
endif()

if (CONFIG_AT_BASE_ON_UART)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_uart_cmd_regist")
endif()

if (CONFIG_AT_SIGNALING_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_fact_cmd_regist")
endif()

if (CONFIG_AT_FS_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_fs_cmd_regist")
endif()

if (CONFIG_AT_DRIVER_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_driver_cmd_regist")
endif()

if (CONFIG_AT_ETHERNET_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_eth_cmd_regist")
endif()

if (CONFIG_AT_USER_COMMAND_SUPPORT)
    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_user_cmd_regist")
endif()
