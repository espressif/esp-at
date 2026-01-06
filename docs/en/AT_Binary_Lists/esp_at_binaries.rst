.. include:: ../../inline_substitutions

Released Firmware
=================

:link_to_translation:`zh_CN:[中文]`

.. list::

  - :ref:`at-firmware-notes`
  :esp32: - :ref:`firmware-esp32-wroom-32-series`
  :esp32: - :ref:`firmware-esp32-mini-1-series`
  :esp32: - :ref:`firmware-esp32-wrover-32-series`
  :esp32: - :ref:`firmware-esp32-pico-series`
  :esp32: - :ref:`firmware-esp32-solo-series`
  :esp32c2: - :ref:`firmware-esp32c2-2mb-g2-series`
  :esp32c2: - :ref:`firmware-esp32c2-4mb-g2-series`
  :esp32c2: - :ref:`firmware-esp32c2-2mb-series`
  :esp32c2: - :ref:`firmware-esp32c2-4mb-series`
  :esp32c3: - :ref:`firmware-esp32c3-mini-1-series`
  :esp32c5: - :ref:`firmware-esp32c5-4mb-series`
  :esp32c6: - :ref:`firmware-esp32c6-4mb-series`
  :esp32c61: - :ref:`firmware-esp32c61-4mb-series`
  :esp32s2: - :ref:`firmware-esp32s2-mini-series`
  - :ref:`at-release-subscription`
  - :ref:`firmware-differences`

.. _at-firmware-notes:

Introduction
^^^^^^^^^^^^

.. note::

  - Before using the AT firmware below, please read the :doc:`../disclaimer` carefully and comply with its terms and precautions.

  .. only:: esp32c2

    - The released AT firmware only supports chip versions with a crystal oscillator frequency of 26 MHz. If your module uses a crystal oscillator frequency of 40 MHz, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

      - ``Component config`` > ``Hardware Settings`` > ``Main XTAL Config`` > ``Main XTAL frequency`` > ``40 MHz``

  .. only:: esp32 or esp32c2

    - If you are unsure which AT firmware your module should use, please read the :ref:`firmware-differences` section first. It compares the various {IDF_TARGET_NAME} AT firmwares in terms of supported command sets, hardware configurations, and module compatibility, helping you determine which firmware best matches your module's hardware configuration.

  .. only:: esp32c3 or esp32c5 or esp32c6 or esp32c61 or esp32s2

    - If you are unsure whether your module can use the default AT firmware, please first read the :ref:`firmware-differences` section. This section describes the supported command sets, hardware configurations, and supported modules of the {IDF_TARGET_NAME} AT firmware, helping you confirm if your module's hardware configuration is suitable for using the default AT firmware.

  - If you need to modify the following configurations, you can generate a new firmware by modifying the AT firmware using the :doc:`../Compile_and_Develop/tools_at_py`.

    .. list::

      - :ref:`at-py-modify-uart`
      - :ref:`at-py-modify-wifi`
      - :ref:`at-py-modify-pki`
      :esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61: - :ref:`at-py-modify-gatts`

.. only:: esp32

  .. _firmware-esp32-wroom-32-series:

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

  .. _firmware-esp32-mini-1-series:

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

  .. _firmware-esp32-pico-series:

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

  .. _firmware-esp32-solo-series:

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

  .. _at-release-subscription:

.. only:: esp32c2

  .. _firmware-esp32c2-2mb-g2-series:

  ESP32-C2-2MB-G2 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  .. note::
    - This series is no longer releasing new AT firmware. It is recommended to use the compatible ESP32-C2-2MB series instead.

  The firmware version below is compatible with {IDF_TARGET_NAME} chip series ECO4 (Rev v2.0) only.

  - v3.3.1.0 `ESP32-C2-2MB-G2-AT-V3.3.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-G2-AT-V3.3.1.0.zip>`__

  .. _firmware-esp32c2-4mb-g2-series:

  ESP32-C2-4MB-G2 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series ECO4 (Rev v2.0) only.

  - v4.1.1.0 `ESP32-C2-4MB-G2-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-C2-4MB-G2-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V4.1.0.0.zip>`__
  - v3.3.1.0 `ESP32-C2-4MB-G2-AT-V3.3.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V3.3.1.0.zip>`__

  .. _firmware-esp32c2-2mb-series:

  ESP32-C2-2MB Series
  ^^^^^^^^^^^^^^^^^^^

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series from ECO1 (Rev v1.0) to ECO4 (Rev v2.0), inclusive of both ECO1 and ECO4 variants.

  - v4.1.1.0 `ESP32-C2-2MB-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-C2-2MB-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V4.1.0.0.zip>`__

  The firmware versions below are compatible with {IDF_TARGET_NAME} chip series ECO1 (Rev v1.0) and ECO2 (Rev v1.1) only.

  - v3.3.0.0 `ESP32-C2-2MB-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.3.0.0.zip>`__
  - v3.1.0.0 `ESP32-C2-2MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.1.0.0.zip>`__
  - v3.0.0.0 `ESP32-C2-2MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.0.0.0.zip>`__

  .. _firmware-esp32c2-4mb-series:

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

  .. _at-release-subscription:

.. only:: esp32c3

  .. _firmware-esp32c3-mini-1-series:

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

  .. _at-release-subscription:

.. only:: esp32c5

  .. _firmware-esp32c5-4mb-series:

  ESP32-C5-4MB Series
  ^^^^^^^^^^^^^^^^^^^

  .. hint::

    - For testing purposes, you may download the ``esp32c5-4MB-at`` firmware from :doc:`GitHub Actions <../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github>`, or :doc:`compile the ESP-AT project locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>`.
    - For production use, please click on the released version below, fill out the AT Firmware Application form, and obtain the AT firmware suitable for the {IDF_TARGET_NAME} series chips.

  **Released version:**

  - `v5.0.0.0 <https://www.espressif.com/en/contact-us/at-firmware-application?chip={IDF_TARGET_NAME}&type={IDF_TARGET_NAME}-4mb&version=v5.0.0.0>`__ (Recommended)

  .. _at-release-subscription:

.. only:: esp32c6

  .. _firmware-esp32c6-4mb-series:

  ESP32-C6-4MB Series
  ^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-C6-4MB-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.1.1.0.zip>`__ (Recommended)
  - v4.1.0.0 `ESP32-C6-4MB-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.1.0.0.zip>`__
  - v4.0.0.0 `ESP32-C6-4MB-AT-V4.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.0.0.0.zip>`__

  .. _at-release-subscription:

.. only:: esp32c61

  .. _firmware-esp32c61-4mb-series:

  ESP32-C61-4MB Series
  ^^^^^^^^^^^^^^^^^^^^

  .. hint::

    - For testing purposes, you may download the ``esp32c61-4MB-at`` firmware from :doc:`GitHub Actions <../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github>`, or :doc:`compile the ESP-AT project locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>`.
    - For production use, please click on the released version below, fill out the AT Firmware Application form, and obtain the AT firmware suitable for the {IDF_TARGET_NAME} series chips.

  **Released version:**

  - `v5.0.0.0 <https://www.espressif.com/en/contact-us/at-firmware-application?chip={IDF_TARGET_NAME}&type={IDF_TARGET_NAME}-4mb&version=v5.0.0.0>`__ (Recommended)

  .. _at-release-subscription:

.. only:: esp32s2

  .. _firmware-esp32s2-mini-series:

  ESP32-S2-MINI Series
  ^^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-S2-MINI-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32s2/ESP32-S2-MINI/ESP32-S2-MINI-AT-V4.1.1.0.zip>`__ (Recommended)
  - v3.4.0.0 `ESP32-S2-MINI-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32s2/ESP32-S2-MINI/ESP32-S2-MINI-AT-V3.4.0.0.zip>`__

  .. _at-release-subscription:

.. only:: placeholder

  .. _at-release-subscription:

Subscribe to AT Releases
^^^^^^^^^^^^^^^^^^^^^^^^

Please refer to the :ref:`sub-at-release` documentation to subscribe to our version release notifications and stay updated on the latest releases.

.. _firmware-differences:

AT Released Firmware Differences
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This section compares the {IDF_TARGET_NAME} AT released firmwares in supported command sets, hardware, and supported modules.

.. only:: esp32

  - ESP32-WROOM-32-AT-Vx.x.x.x.zip (referred to as **WROOM Bin** in this section);
  - ESP32-WROVER-32-AT-Vx.x.x.x.zip (referred to as **WROVER Bin** in this section);
  - ESP32-PICO-D4-AT-Vx.x.x.x.zip (referred to as **PICO-D4 Bin** in this section);
  - ESP32-SOLO-1-AT-Vx.x.x.x.zip (referred to as **SOLO-1 Bin** in this section);
  - ESP32-MINI-1-AT-Vx.x.x.x.zip (referred to as **MINI-1 Bin** in this section);
  - ESP32-D2WD-AT-Vx.x.x.x.zip (referred to as **D2WD Bin** in this section);

.. only:: esp32c2

  - ESP32C2-2MB-AT-Vx.x.x.x.zip (referred to as **ESP32C2-2MB Bin** in this section);
  - ESP32C2-4MB-AT-Vx.x.x.x.zip (referred to as **ESP32C2-4MB Bin** in this section);
  - ESP32C2-2MB-G2-AT-Vx.x.x.x.zip (referred to as **ESP32C2-2MB-G2 Bin** in this section);
  - ESP32C2-4MB-G2-AT-Vx.x.x.x.zip (referred to as **ESP32C2-4MB-G2 Bin** in this section);
  - ESP32C2-2MB-BLE Bin (firmware is not released, but you can download the ``esp32c2-2mb-ble-at`` firmware from :doc:`GitHub Actions <../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github>`, or :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` by yourself);

.. only:: esp32c3

  - ESP32-C3-MINI-1-AT-Vx.x.x.x.zip (referred to as **MINI-1 Bin** in this section);

.. only:: esp32c5 or esp32c6 or esp32c61

  - {IDF_TARGET_NAME}-4MB-AT-Vx.x.x.x.zip (referred to as **{IDF_TARGET_NAME}-4MB Bin** in this section);

.. only:: esp32s2

  - ESP32-S2-MINI-AT-Vx.x.x.x.zip (referred to as **MINI Bin** in this section);

Supported Command Set
---------------------

The table lists which command set is supported by default in the official AT firmware applicable to {IDF_TARGET_NAME} series of modules (marked with |icon-green-check|), which is not supported by default but can be supported after configuration of the ESP-AT project (marked with |icon-orange-check|), and which is not supported at all (marked with |icon-red-cross|). Note that the command set that is not shown in this table is not supported either. Applicable firmware that has not been :doc:`officially released <../AT_Binary_Lists/index>` requires compilation by yourself. Those self-compiled firmware cannot be upgraded OTA from Espressif official server.

.. only:: esp32

  .. list-table::
    :header-rows: 1

    * - Command Set
      - WROOM Bin
      - WROVER Bin
      - PICO-D4 Bin
      - SOLO-1 Bin
      - MINI-1 Bin
      - D2WD Bin
    * - base
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - user
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - TCP-IP
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - mDNS
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - WPS
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - SmartConfig
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ping
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - MQTT
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - HTTP
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Bluetooth LE
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Bluetooth LE HID
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - BluFi
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Bluetooth SPP
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - Bluetooth A2DP
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - ethernet
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - FileSystem
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - Web server
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - WebSocket
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - OTA
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|

.. only:: esp32c2

  .. list-table::
    :header-rows: 1

    * - Command Set
      - ESP32C2-2MB Bin
      - ESP32C2-4MB Bin
      - ESP32C2-2MB-G2 Bin
      - ESP32C2-4MB-G2 Bin
      - ESP32C2-2MB-BLE Bin
    * - base
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - user
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - TCP-IP
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - mDNS
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - WPS
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - SmartConfig
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - ping
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - MQTT
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - HTTP
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - Bluetooth LE
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-green-check|
    * - BluFi
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-red-cross|
    * - FileSystem
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-red-cross|
    * - WEB server
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-red-cross|
    * - WebSocket
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-red-cross|
    * - OTA
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|

.. only:: esp32c3

  .. list-table::
    :header-rows: 1

    * - Command Set
      - MINI-1 Bin
    * - base
      - |icon-green-check|
    * - user
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
    * - TCP-IP
      - |icon-green-check|
    * - mDNS
      - |icon-green-check|
    * - WPS
      - |icon-green-check|
    * - SmartConfig
      - |icon-green-check|
    * - ping
      - |icon-green-check|
    * - MQTT
      - |icon-green-check|
    * - HTTP
      - |icon-green-check|
    * - Bluetooth LE
      - |icon-green-check|
    * - Bluetooth LE HID
      - |icon-green-check|
    * - BluFi
      - |icon-green-check|
    * - FileSystem
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
    * - Web server
      - |icon-orange-check|
    * - WebSocket
      - |icon-orange-check|
    * - OTA
      - |icon-green-check|

.. only:: esp32c5 or esp32c6 or esp32c61

  .. list-table::
    :header-rows: 1

    * - Command Set
      - {IDF_TARGET_NAME}-4MB Bin
    * - base
      - |icon-green-check|
    * - user
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
    * - TCP-IP
      - |icon-green-check|
    * - mDNS
      - |icon-green-check|
    * - WPS
      - |icon-green-check|
    * - SmartConfig
      - |icon-green-check|
    * - ping
      - |icon-green-check|
    * - MQTT
      - |icon-green-check|
    * - HTTP
      - |icon-green-check|
    * - Bluetooth LE
      - |icon-green-check|
    * - BluFi
      - |icon-green-check|
    * - FileSystem
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
    * - Web server
      - |icon-orange-check|
    * - WebSocket
      - |icon-orange-check|
    * - OTA
      - |icon-green-check|

.. only:: esp32s2

  .. list-table::
    :header-rows: 1

    * - Command Set
      - MINI Bin
    * - base
      - |icon-green-check|
    * - user
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
    * - TCP-IP
      - |icon-green-check|
    * - mDNS
      - |icon-green-check|
    * - WPS
      - |icon-green-check|
    * - SmartConfig
      - |icon-green-check|
    * - ping
      - |icon-green-check|
    * - MQTT
      - |icon-green-check|
    * - HTTP
      - |icon-green-check|
    * - FileSystem
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
    * - Web server
      - |icon-orange-check|
    * - WebSocket
      - |icon-orange-check|
    * - OTA
      - |icon-green-check|

Hardware Differences
--------------------

.. only:: esp32

  .. list-table::
    :header-rows: 1

    * - Hardware
      - WROOM Bin
      - WROVER Bin
      - PICO-D4 Bin
      - SOLO-1 Bin
      - MINI-1 Bin
      - D2WD Bin
    * - Flash
      - 4 MB
      - 4 MB
      - 4 MB
      - 4 MB
      - 4 MB
      - 2 MB
    * - PSRAM
      - |icon-red-cross|
      - 8 MB
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 17
        | RX: 16
        | CTS: 15
        | RTS: 14
      - | TX: 22
        | RX: 19
        | CTS: 15
        | RTS: 14
      - | TX: 22
        | RX: 19
        | CTS: 15
        | RTS: 14
      - | TX: 17
        | RX: 16
        | CTS: 15
        | RTS: 14
      - | TX: 22
        | RX: 19
        | CTS: 15
        | RTS: 14
      - | TX: 22
        | RX: 19
        | CTS: 15
        | RTS: 14

.. only:: esp32c2

  .. list-table::
    :header-rows: 1

    * - Hardware
      - ESP32C2-2MB Bin
      - ESP32C2-4MB Bin
      - ESP32C2-2MB-G2 Bin
      - ESP32C2-4MB-G2 Bin
      - ESP32C2-2MB-BLE Bin
    * - Flash
      - 2 MB
      - 4 MB
      - 2 MB
      - 4 MB
      - 2 MB
    * - PSRAM
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 19
        | RTS: 20
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4
      - | TX: 7
        | RX: 6
        | CTS: 19
        | RTS: 20
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32c3

  .. list-table::
    :header-rows: 1

    * - Hardware
      - MINI-1 Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32c5

  .. list-table::
    :header-rows: 1

    * - Hardware
      - {IDF_TARGET_NAME}-4MB Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 23
        | RX: 24
        | CTS: 25
        | RTS: 26

.. only:: esp32c6

  .. list-table::
    :header-rows: 1

    * - Hardware
      - {IDF_TARGET_NAME}-4MB Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32c61

  .. list-table::
    :header-rows: 1

    * - Hardware
      - {IDF_TARGET_NAME}-4MB Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32s2

  .. list-table::
    :header-rows: 1

    * - Hardware
      - MINI Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 17
        | RX: 21
        | CTS: 20
        | RTS: 19

.. [#one] UART pins can be customized. See :doc:`../Compile_and_Develop/How_to_set_AT_port_pin` for details.

.. _support-modules:

Supported Modules
-----------------

The table below lists the modules or chips that are default supported by the officially released {IDF_TARGET_NAME} series AT firmware (indicated by |icon-green-check|), the modules that are not supported by default but can be modified to be supported using the :doc:`../Compile_and_Develop/tools_at_py` (indicated by |icon-orange-check|), and the modules that are not supported at all (indicated by |icon-red-cross|). For modules that are not supported at all, you can refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it` to modify the configuration as needed for support.

.. only:: esp32

  .. list-table::
    :header-rows: 1
    :widths: 100 40 40 40 40 40 40

    * - Module/Chip
      - WROOM Bin
      - WROVER Bin
      - PICO-D4 Bin
      - SOLO-1 Bin
      - MINI-1 Bin
      - D2WD Bin
    * - ESP32-WROOM-32
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32D
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32E
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32U
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32UE
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32DA
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32SE
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER-E
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER-IE
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER-B
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER-IB
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-SOLO-1
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-D2WD
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-green-check|
    * - ESP32-MINI-1
      - |icon-orange-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-MINI-1U
      - |icon-orange-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-PICO-D4
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-PICO-V3-ZERO
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
    * - ESP32-PICO-MINI-02
      - |icon-orange-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-orange-check|
    * - ESP32-PICO-MINI-02U
      - |icon-orange-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-orange-check|

.. only:: esp32c2

  .. list-table::
    :header-rows: 1
    :widths: 100 60 60 60 60 60

    * - Module/Chip
      - ESP32C2-2MB Bin
      - ESP32C2-4MB Bin
      - ESP32C2-2MB-G2 Bin
      - ESP32C2-4MB-G2 Bin
      - ESP32C2-2MB-BLE Bin
    * - ESP8684-MINI-1/1U
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-02C/02UC
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-03
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-05
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-07
      - |icon-red-cross|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|

.. only:: esp32c3

  .. list-table::
    :header-rows: 1
    :widths: 100 60

    * - Module/Chip
      - MINI-1 Bin
    * - ESP32-C3-MINI-1/1U
      - |icon-green-check|
    * - ESP32-C3-WROOM-02/02U
      - |icon-green-check|
    * - ESP8685-WROOM-01
      - |icon-green-check|
    * - ESP8685-WROOM-03
      - |icon-green-check|
    * - ESP8685-WROOM-04
      - |icon-green-check|
    * - ESP8685-WROOM-05
      - |icon-green-check|
    * - ESP8685-WROOM-06
      - |icon-green-check|
    * - ESP8685-WROOM-07
      - |icon-orange-check|

.. only:: esp32c5

  .. list-table::
    :header-rows: 1

    * - Module/Chip
      - {IDF_TARGET_NAME}-4MB Bin
    * - {IDF_TARGET_NAME}-MINI-1
      - |icon-green-check|
    * - {IDF_TARGET_NAME}-WROOM-01
      - |icon-green-check|

.. only:: esp32c6

  .. list-table::
    :header-rows: 1

    * - Module/Chip
      - {IDF_TARGET_NAME}-4MB Bin
    * - {IDF_TARGET_NAME}-MINI-1
      - |icon-green-check|
    * - {IDF_TARGET_NAME}-WROOM-01
      - |icon-green-check|

.. only:: esp32c61

  .. list-table::
    :header-rows: 1

    * - Module/Chip
      - {IDF_TARGET_NAME}-4MB Bin
    * - {IDF_TARGET_NAME}-WROOM-01
      - |icon-green-check|

.. only:: esp32s2

  .. list-table::
    :header-rows: 1

    * - Module/Chip
      - MINI Bin
    * - ESP32-S2-MINI-2/2U
      - |icon-green-check|
    * - ESP32-S2-SOLO-2/2U
      - |icon-green-check|
