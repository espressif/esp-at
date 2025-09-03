###############################
# setup environment variables #
###############################
function setup_env_variables() {
  # module info
  mkdir build
  echo -e "{\"platform\": \"$PLATFORM\", \"module\": \"$MODULE_NAME\", \"silence\": $SILENCE}" > build/module_info.json

  # module config directory
  module_name_lower=$(echo "${MODULE_NAME}" | tr '[:upper:]' '[:lower:]')
  module_cfg_dir="${GITHUB_WORKSPACE}/module_config/module_${module_name_lower}"
  if [ ! -d "${module_cfg_dir}" ]; then
      platform_name_str=$(echo "${PLATFORM}" | sed 's/PLATFORM_//')
      module_name_lower=$(echo "${platform_name_str}" | tr '[:upper:]' '[:lower:]')
      module_cfg_dir="${GITHUB_WORKSPACE}/module_config/module_${module_name_lower}_default"
  else
      module_cfg_dir="${GITHUB_WORKSPACE}/module_config/module_${module_name_lower}"
  fi
  echo "current configuration dir: ${module_cfg_dir}"
  echo "module_cfg_dir=${module_cfg_dir}" >> $GITHUB_ENV

  # sdkconfig file
  if [ "$SILENCE" = "0" ]; then
      at_sdkconfig_file="${module_cfg_dir}/sdkconfig.defaults"
  elif [ "$SILENCE" = "1" ]; then
      at_sdkconfig_file="${module_cfg_dir}/sdkconfig_silence.defaults"
  else
      at_sdkconfig_file="na"
  fi
  echo "current sdkconfig file: ${at_sdkconfig_file}"
  echo "at_sdkconfig_file=${at_sdkconfig_file}" >> $GITHUB_ENV

  # firmware source
  echo "AT firmware from ${GITHUB_SERVER_URL}/${GITHUB_REPOSITORY}"
  echo "ESP_AT_FIRMWARE_FROM=${GITHUB_SERVER_URL}/${GITHUB_REPOSITORY}" >> "$GITHUB_ENV"
}

#############################
# Update sdkconfig.defaults #
#############################
function enable_at_debug() {
  echo -e "CONFIG_LOG_DEFAULT_LEVEL_DEBUG=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_MBEDTLS_DEBUG=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_DEBUG=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_NET_DEBUG=y" >> ${at_sdkconfig_file}
}

function enable_all_wifi_cmds() {
  echo -e "CONFIG_AT_WS_COMMAND_SUPPORT=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_FS_COMMAND_SUPPORT=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_DRIVER_COMMAND_SUPPORT=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_EAP_COMMAND_SUPPORT=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_WEB_SERVER_SUPPORT=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE=y" >> ${at_sdkconfig_file}
}

function disable_all_wifi_cmds() {
  echo -e "CONFIG_AT_WS_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_FS_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_ETHERNET_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_EAP_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_DRIVER_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_WEB_SERVER_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_OTA_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_MDNS_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_WPS_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_SMARTCONFIG_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_PING_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_MQTT_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_HTTP_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_SIGNALING_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_ESP_WIFI_SLP_IRAM_OPT=n" >> ${at_sdkconfig_file}
}

function enable_ethernet_cmds() {
  echo -e "CONFIG_AT_ETHERNET_SUPPORT=y" >> ${at_sdkconfig_file}
}

function enable_bt_cmds() {
  echo -e "CONFIG_AT_BT_COMMAND_SUPPORT=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_BT_SPP_COMMAND_SUPPORT=y" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_BT_A2DP_COMMAND_SUPPORT=y" >> ${at_sdkconfig_file}
}

function enable_ble_cmds() {
  echo -e "CONFIG_AT_BLE_COMMAND_SUPPORT=y" >> ${at_sdkconfig_file}
}

function disable_ble_cmds() {
  echo -e "CONFIG_AT_BLE_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_BLE_HID_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_BLUFI_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
  echo -e "CONFIG_AT_BT_COMMAND_SUPPORT=n" >> ${at_sdkconfig_file}
}

#########################
# Enlarge app partition #
#########################
function enlarge_app_partition() {
  app0_size=`cat ${module_cfg_dir}/partitions_at.csv | grep ota_0 | awk -F, '{print $5}'`
  app1_size=`cat ${module_cfg_dir}/partitions_at.csv | grep ota_1 | awk -F, '{print $5}'`
  to_set_size=$((app0_size + app1_size))
  sed -i '/ota_1/d' ${module_cfg_dir}/partitions_at.csv
  to_set_hex_size=`printf "0x%x" ${to_set_size}`
  sed -i '/ota_0/s,'"$app0_size"','"$to_set_hex_size"',g' ${module_cfg_dir}/partitions_at.csv
}
