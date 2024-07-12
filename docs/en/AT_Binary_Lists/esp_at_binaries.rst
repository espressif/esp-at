Released Firmware
=================

:link_to_translation:`zh_CN:[中文]`

It is recommended to use the lastest version of firmware. Currently, Espressif releases AT firmware for the following {IDF_TARGET_NAME} series of modules.

.. note::

  .. only:: esp32c2

    - The released AT firmware only supports chip versions with a crystal oscillator frequency of 26 MHz. If your module uses a crystal oscillator frequency of 40 MHz, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

      - ``Component config`` -> ``Hardware Settings`` -> ``Main XTAL Config`` -> ``Main XTAL frequency`` -> ``40 MHz``

  .. only:: esp32c3

    - The AT firmware versions v2.2.0.0 to v3.2.0.0 support the {IDF_TARGET_NAME} ECO0 (Rev v0.0) to ECO4 (Rev v0.4) series chips (including ECO0 and ECO4).
    - The AT firmware version v3.3.0.0 supports the {IDF_TARGET_NAME} ECO0 (Rev v0.0) to ECO4 (Rev v0.4), ECO6 (Rev v1.0), and ECO7 (Rev v1.1) series chips.

  .. only:: esp32 or esp32c2

    - If you are unsure which AT firmware your module should use, please read the :doc:`../Compile_and_Develop/esp-at_firmware_differences` document first. This document compares the differences among different {IDF_TARGET_NAME} AT firmwares in terms of supported command sets, hardware configurations, and module compatibility, helping you determine if there is a suitable firmware for your module's hardware configuration.

  .. only:: esp32c3 or esp32c6 or esp32s2

    - If you are unsure whether your module can use the default AT firmware, please read the :doc:`../Compile_and_Develop/esp-at_firmware_differences` document first. This document compares the differences among different {IDF_TARGET_NAME} AT firmwares in terms of supported command sets, hardware configurations, and module compatibility, helping you confirm if your module's hardware configuration is suitable for using the default AT firmware.

  - If you need to modify the following configurations, you can generate a new firmware by modifying the AT firmware using the :doc:`../Compile_and_Develop/tools_at_py`.

    .. list::

      - :ref:`at-py-modify-uart`
      - :ref:`at-py-modify-wifi`
      - :ref:`at-py-modify-pki`
      :esp32 or esp32c2 or esp32c3 or esp32c6: - :ref:`at-py-modify-gatts`

  .. only:: esp32c2

    - Currently, {IDF_TARGET_NAME} AT released firmware does not support most of :ref:`Bluetooth® Low Energy AT Commands <BLE-AT>` (excluding BluFi commands). If you only need to use :ref:`Basic AT Commands <Basic-AT>` and :ref:`Bluetooth® Low Energy AT Commands <BLE-AT>` on {IDF_TARGET_NAME}, follow the steps described in :doc:`How_to_download_the_latest_temporary_version_of_AT_from_github <../Compile_and_Develop/How_to_build_project_with_web_page>` to directly download the esp32c2-ble-2mb-at firmware, or compile this firmware by yourself following the steps in the :doc:`How_to_clone_project_and_compile_it <../Compile_and_Develop/How_to_clone_project_and_compile_it>` (Note: When setting up the environment in step three of the document, choose ESP32C2-BLE-2MB).

        - This firmware currently does not support OTA function.
        - AT firmware on GitHub will automatically expire after 90 days, so please save the downloaded AT firmware.

.. only:: esp32

  ESP32-WROOM-32 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  - v3.4.0.0 `ESP32-WROOM-32-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V3.4.0.0.zip>`__ (Recommended)
  - v3.2.0.0 `ESP32-WROOM-32-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-WROOM-32-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-WROOM-32-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-WROOM-32-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-WROOM-32-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V2.0.0.0.zip>`__
  - v1.1.2.0 `ESP32-WROOM-32-AT-V1.1.2.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V1.1.2.0.zip>`__
  - v1.1.1.0 `ESP32-WROOM-32-AT-V1.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V1.1.1.0.zip>`__
  - v1.1.0.0 `ESP32-WROOM-32-AT-V1.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V1.1.0.0.zip>`__
  - v1.0.0.0 `ESP32-WROOM-32-AT-V1.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V1.0.0.0.zip>`__
  - v0.10.0.0 `ESP32-WROOM-32-AT-V0.10.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V0.10.0.0.zip>`__

  ESP32-MINI-1 Series
  ^^^^^^^^^^^^^^^^^^^

  - v3.4.0.0 `ESP32-MINI-1-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.4.0.0.zip>`__ (Recommended)
  - v3.2.0.0 `ESP32-MINI-1-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-MINI-1-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-MINI-1-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V2.2.0.0.zip>`__

  .. _firmware-esp32-wrover-32-series:

  ESP32-WROVER-32 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  It is not recommended to use the ESP32-WROVER-B module due to hardware limit. Please use other WROVER series modules.

  - v2.4.0.0 `ESP32-WROVER-32-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.4.0.0.zip>`__ (Recommended)
  - v2.2.0.0 `ESP32-WROVER-32-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-WROVER-32-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-WROVER-32-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.0.0.0.zip>`__
  - v0.10.0.0 `ESP32-WROVER-32-AT-V0.10.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V0.10.0.0.zip>`__

  ESP32-PICO Series
  ^^^^^^^^^^^^^^^^^
 
  - v3.4.0.0 `ESP32-PICO-D4-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.4.0.0.zip>`__ (Recommended)
  - v3.2.0.0 `ESP32-PICO-D4-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-PICO-D4-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-PICO-D4-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-PICO-D4-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-PICO-D4-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.0.0.0.zip>`__

  ESP32-SOLO Series
  ^^^^^^^^^^^^^^^^^

  - v3.4.0.0 `ESP32-SOLO-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.4.0.0.zip>`__ (Recommended)
  - v3.2.0.0 `ESP32-SOLO-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-SOLO-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-SOLO-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-SOLO-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-SOLO-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.0.0.0.zip>`__

.. only:: esp32c2

  ESP32-C2 2MB Series
  ^^^^^^^^^^^^^^^^^^^

  - v3.3.0.0 `ESP32-C2-2MB-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.3.0.0.zip>`__ (Recommended)
  - v3.1.0.0 `ESP32-C2-2MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.1.0.0.zip>`__
  - v3.0.0.0 `ESP32-C2-2MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.0.0.0.zip>`__

  ESP32-C2 4MB Series
  ^^^^^^^^^^^^^^^^^^^

  - v3.3.0.0 `ESP32-C2-4MB-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.3.0.0.zip>`__ (Recommended)
  - v3.1.0.0 `ESP32-C2-4MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.1.0.0.zip>`__
  - v3.0.0.0 `ESP32-C2-4MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.0.0.0.zip>`__

.. only:: esp32c3

  ESP32-C3-MINI-1 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  - v3.3.0.0 `ESP32-C3-MINI-1-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V3.3.0.0.zip>`__ (Recommended)
  - v3.2.0.0 `ESP32-C3-MINI-1-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V3.2.0.0.zip>`__
  - v2.4.2.0 `ESP32-C3-MINI-1-AT-V2.4.2.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.2.0.zip>`__
  - v2.4.1.0 `ESP32-C3-MINI-1-AT-V2.4.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.1.0.zip>`__
  - v2.4.0.0 `ESP32-C3-MINI-1-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.0.0.zip>`__
  - v2.3.0.0 `ESP32-C3-MINI-1-AT-V2.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.3.0.0.zip>`__
  - v2.2.0.0 `ESP32-C3-MINI-1-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.2.0.0.zip>`__

.. only:: esp32c6

  ESP32-C6 4MB Series
  ^^^^^^^^^^^^^^^^^^^

  - v4.0.0.0 `ESP32-C6-4MB-AT-V4.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.0.0.0.zip>`__ (Recommended)

.. only:: esp32s2

  ESP32-S2-MINI Series
  ^^^^^^^^^^^^^^^^^^^^

  - v3.4.0.0 `ESP32-S2-MINI-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32s2/ESP32-S2-MINI/ESP32-S2-MINI-AT-V3.4.0.0.zip>`__ (Recommended)

Subscribe to AT Releases
^^^^^^^^^^^^^^^^^^^^^^^^

Please refer to the :ref:`sub-at-release` documentation to subscribe to our version release notifications and stay updated on the latest releases.
