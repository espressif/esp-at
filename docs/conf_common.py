# -*- coding: utf-8 -*-
#
# Common (non-language-specific) configuration for Sphinx
#
# This file is imported from a language-specific conf.py (ie en/conf.py or
# zh_CN/conf.py)

from __future__ import print_function, unicode_literals

import os.path

#ESP_DOCS_PATH = os.environ['ESP_DOCS_PATH']

try:
    from esp_docs.conf_docs import *  # noqa: F403,F401
except ImportError:
    import os
    import sys
    sys.path.insert(0, os.path.abspath(ESP_DOCS_PATH))
    from conf_docs import *  # noqa: F403,F401

ESP32_DOCS = ['AT_Binary_Lists/ESP32_AT_binaries.rst',
              'AT_Command_Examples/Ethernet_AT_Examples.rst',
              'AT_Command_Examples/classic_bluetooth_at_examples.rst',
              'AT_Command_Examples/bluetooth_le_at_examples.rst',
              'AT_Command_Set/BT_AT_Commands.rst',
              'AT_Command_Set/BLE_AT_Commands.rst',
              'AT_Command_Set/Ethernet_AT_Commands.rst',
              'Compile_and_Develop/How_to_customize_BLE_services.rst',
              'Compile_and_Develop/customize_bluetooth_le_services_tools.rst',
              'Compile_and_Develop/esp32-sdio-at-guide.rst',
              'Compile_and_Develop/how_to_enable_at_classic_bluetooth.rst',
              'Compile_and_Develop/How_to_enable_ESP_AT_Ethernet.rst',
              'Customized_AT_Commands_and_Firmware/*'
              ]

ESP32C3_DOCS = ['AT_Binary_Lists/ESP32-C3_AT_binaries.rst',
                'AT_Command_Examples/bluetooth_le_at_examples.rst',
                'AT_Command_Set/BLE_AT_Commands.rst',
                'Compile_and_Develop/How_to_customize_BLE_services.rst',
                'Compile_and_Develop/customize_bluetooth_le_services_tools.rst',
                'Compile_and_Develop/How_to_implement_SPI_AT.rst',
                'Customized_AT_Commands_and_Firmware/*'
                ]

ESP32C2_DOCS = ['AT_Binary_Lists/ESP32-C2_AT_binaries.rst']

# reserved item
ESP8266_DOCS = []

ESP32S2_DOCS = []

# format: {tag needed to include: documents to included}, tags are parsed from sdkconfig and peripheral_caps.h headers
conditional_include_dict = {
                            'esp32':ESP32_DOCS,
                            'esp32c2':ESP32C2_DOCS,
                            'esp32c3':ESP32C3_DOCS,
                            'esp8266':ESP8266_DOCS,
                            'esp32s2':ESP32S2_DOCS
                            }

extensions += ['sphinx_copybutton',
               # Note: order is important here, events must
               # be registered by one extension before they can be
               # connected to another extension
               'esp_docs.esp_extensions.dummy_build_system',
               'esp_docs.esp_extensions.run_doxygen',
               ]

# link roles config
github_repo = 'espressif/esp-at'

# context used by sphinx_idf_theme
html_context['github_user'] = 'espressif'
html_context['github_repo'] = 'esp-at'

idf_targets = ['esp32', 'esp32c2', 'esp32c3', 'esp8266', 'esp32s2']
languages = ['en', 'zh_CN']

google_analytics_id = os.environ.get('CI_GOOGLE_ANALYTICS_ID', None)

project_homepage = 'https://github.com/espressif/esp-at'

html_static_path = ['../_static']

# Extra options required by sphinx_idf_theme
project_slug = 'esp-at'

versions_url = './_static/js/at_versions.js'

# Final PDF filename will contains target and version
pdf_file_prefix = u'esp-at'
