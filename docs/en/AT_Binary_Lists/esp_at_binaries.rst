Released Firmware
=================

:link_to_translation:`zh_CN:[中文]`

It is recommended to use the lastest version of firmware. Currently, Espressif releases AT firmware for the following {IDF_TARGET_NAME} series of modules.

.. note::

  .. only:: esp32c2

    - The released AT firmware only supports chip versions with a crystal oscillator frequency of 26 MHz. If your module uses a crystal oscillator frequency of 40 MHz, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

      - ``Component config`` -> ``Hardware Settings`` -> ``Main XTAL Config`` -> ``Main XTAL frequency`` -> ``40 MHz``

  .. only:: esp32 or esp32c2

    - If you are unsure which AT firmware your module should use, please read the :doc:`../Compile_and_Develop/esp-at_firmware_differences` document first. This document compares the differences among different {IDF_TARGET_NAME} AT firmwares in terms of supported command sets, hardware configurations, and module compatibility, helping you determine if there is a suitable firmware for your module's hardware configuration.

  .. only:: esp32c3 or esp32c5 or esp32c6 or esp32s2

    - If you are unsure whether your module can use the default AT firmware, please read the :doc:`../Compile_and_Develop/esp-at_firmware_differences` document first. This document compares the differences among different {IDF_TARGET_NAME} AT firmwares in terms of supported command sets, hardware configurations, and module compatibility, helping you confirm if your module's hardware configuration is suitable for using the default AT firmware.

  - If you need to modify the following configurations, you can generate a new firmware by modifying the AT firmware using the :doc:`../Compile_and_Develop/tools_at_py`.

    .. list::

      - :ref:`at-py-modify-uart`
      - :ref:`at-py-modify-wifi`
      - :ref:`at-py-modify-pki`
      :esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6: - :ref:`at-py-modify-gatts`

Disclaimer
^^^^^^^^^^

Before using the AT firmware below, please read the :doc:`../disclaimer` carefully and comply with its terms and precautions.

.. only:: esp32

  ESP32-WROOM-32 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-WROOM-32-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-WROOM-32-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V4.1.0.0.zip>`__
  - v3.4.0.0 `ESP32-WROOM-32-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V3.4.0.0.zip>`__
  - v3.2.1.0 `ESP32-WROOM-32-AT-V3.2.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V3.2.1.0.zip>`__
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

  - v4.1.1.0 `ESP32-MINI-1-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-MINI-1-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V4.1.0.0.zip>`__
  - v3.4.0.0 `ESP32-MINI-1-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.4.0.0.zip>`__
  - v3.2.1.0 `ESP32-MINI-1-AT-V3.2.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.2.1.0.zip>`__
  - v3.2.0.0 `ESP32-MINI-1-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-MINI-1-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-MINI-1-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V2.2.0.0.zip>`__

  .. _firmware-esp32-wrover-32-series:

  ESP32-WROVER-32 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  .. note::
    - This series is no longer releasing new AT firmware. It is recommended to use the ESP32-MINI-1 series instead.
    - It is not recommended to use the ESP32-WROVER-B module due to hardware limit. Please use other WROVER series modules.

  - v2.4.0.0 `ESP32-WROVER-32-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-WROVER-32-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-WROVER-32-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-WROVER-32-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.0.0.0.zip>`__
  - v0.10.0.0 `ESP32-WROVER-32-AT-V0.10.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V0.10.0.0.zip>`__

  ESP32-PICO Series
  ^^^^^^^^^^^^^^^^^
 
  - v4.1.1.0 `ESP32-PICO-D4-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-PICO-D4-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V4.1.0.0.zip>`__
  - v3.4.0.0 `ESP32-PICO-D4-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.4.0.0.zip>`__
  - v3.2.1.0 `ESP32-PICO-D4-AT-V3.2.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.2.1.0.zip>`__
  - v3.2.0.0 `ESP32-PICO-D4-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-PICO-D4-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-PICO-D4-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-PICO-D4-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-PICO-D4-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.0.0.0.zip>`__

  ESP32-SOLO Series
  ^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-SOLO-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-SOLO-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V4.1.0.0.zip>`__
  - v3.4.0.0 `ESP32-SOLO-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.4.0.0.zip>`__
  - v3.2.1.0 `ESP32-SOLO-AT-V3.2.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.2.1.0.zip>`__
  - v3.2.0.0 `ESP32-SOLO-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-SOLO-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-SOLO-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-SOLO-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-SOLO-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.0.0.0.zip>`__

.. only:: esp32c2

  ESP32-C2-2MB-G2 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  .. note::
    - This series is no longer releasing new AT firmware. It is recommended to use the compatible ESP32-C2-2MB series instead.

  The firmware version below is compatible with {IDF_TARGET_NAME} chip series ECO4 (Rev v2.0) only.

  - v3.3.1.0 `ESP32-C2-2MB-G2-AT-V3.3.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-G2-AT-V3.3.1.0.zip>`__

  ESP32-C2-4MB-G2 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series ECO4 (Rev v2.0) only.

  - v4.1.1.0 `ESP32-C2-4MB-G2-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-C2-4MB-G2-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V4.1.0.0.zip>`__
  - v3.3.1.0 `ESP32-C2-4MB-G2-AT-V3.3.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V3.3.1.0.zip>`__

  ESP32-C2-2MB Series
  ^^^^^^^^^^^^^^^^^^^

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series from ECO1 (Rev v1.0) to ECO4 (Rev v2.0), inclusive of both ECO1 and ECO4 variants.

  - v4.1.1.0 `ESP32-C2-2MB-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-C2-2MB-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V4.1.0.0.zip>`__

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series ECO1 (Rev v1.0) and ECO2 (Rev v1.1) only.

  - v3.3.0.0 `ESP32-C2-2MB-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.3.0.0.zip>`__
  - v3.1.0.0 `ESP32-C2-2MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.1.0.0.zip>`__
  - v3.0.0.0 `ESP32-C2-2MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.0.0.0.zip>`__

  ESP32-C2-4MB Series
  ^^^^^^^^^^^^^^^^^^^

  .. note::
    - If your hardware is ECO4 (Rev v2.0) chip series, it is recommended to use the ESP32-C2-4MB-G2 series instead.

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series from ECO1 (Rev v1.0) to ECO4 (Rev v2.0), inclusive of both ECO1 and ECO4 variants.

  - v4.1.1.0 `ESP32-C2-4MB-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-C2-4MB-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V4.1.0.0.zip>`__

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series ECO1 (Rev v1.0) and ECO2 (Rev v1.1) only.

  - v3.3.0.0 `ESP32-C2-4MB-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.3.0.0.zip>`__
  - v3.1.0.0 `ESP32-C2-4MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.1.0.0.zip>`__
  - v3.0.0.0 `ESP32-C2-4MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.0.0.0.zip>`__

.. only:: esp32c3

  ESP32-C3-MINI-1 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series from ECO0 (Rev v0.0) to ECO7 (Rev v1.1), inclusive of both ECO0 and ECO7 variants.

  - v4.1.1.0 `ESP32-C3-MINI-1-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-C3-MINI-1-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V4.1.0.0.zip>`__
  - v3.3.0.0 `ESP32-C3-MINI-1-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V3.3.0.0.zip>`__

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series from ECO0 (Rev v0.0) to ECO4 (Rev v0.4), inclusive of both ECO0 and ECO4 variants.

  - v3.2.0.0 `ESP32-C3-MINI-1-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V3.2.0.0.zip>`__
  - v2.4.2.0 `ESP32-C3-MINI-1-AT-V2.4.2.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.2.0.zip>`__
  - v2.4.1.0 `ESP32-C3-MINI-1-AT-V2.4.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.1.0.zip>`__
  - v2.4.0.0 `ESP32-C3-MINI-1-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.0.0.zip>`__
  - v2.3.0.0 `ESP32-C3-MINI-1-AT-V2.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.3.0.0.zip>`__
  - v2.2.0.0 `ESP32-C3-MINI-1-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.2.0.0.zip>`__

.. only:: esp32c5

  ESP32-C5-4MB Series
  ^^^^^^^^^^^^^^^^^^^

  The first version of ESP-AT firmware is planned to be released in October 2025.

.. only:: esp32c6

  ESP32-C6-4MB Series
  ^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-C6-4MB-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-C6-4MB-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.1.0.0.zip>`__
  - v4.0.0.0 `ESP32-C6-4MB-AT-V4.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.0.0.0.zip>`__

.. only:: esp32s2

  ESP32-S2-MINI Series
  ^^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-S2-MINI-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32s2/ESP32-S2-MINI/ESP32-S2-MINI-AT-V4.1.1.0.zip>`__ (Recommended)
  - v3.4.0.0 `ESP32-S2-MINI-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32s2/ESP32-S2-MINI/ESP32-S2-MINI-AT-V3.4.0.0.zip>`__

Subscribe to AT Releases
^^^^^^^^^^^^^^^^^^^^^^^^

Please refer to the :ref:`sub-at-release` documentation to subscribe to our version release notifications and stay updated on the latest releases.
