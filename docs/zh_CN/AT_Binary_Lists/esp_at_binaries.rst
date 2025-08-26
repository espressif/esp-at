发布的固件
===========

:link_to_translation:`en:[English]`

推荐下载最新版本的固件。目前，乐鑫发布了以下 {IDF_TARGET_NAME} 系列模组的 AT 固件。

.. note::

  .. only:: esp32c2

    - 发布的 AT 固件仅支持 26 MHz 晶振频率的芯片版本。如果您的模组使用的是 40 MHz 的晶振频率，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

      - ``Component config`` -> ``Hardware Settings`` -> ``Main XTAL Config`` -> ``Main XTAL frequency`` -> ``40 MHz``

  .. only:: esp32 or esp32c2

    - 如果您不确定自己的模组应该使用哪个 AT 固件，请先阅读 :doc:`../Compile_and_Develop/esp-at_firmware_differences` 文档，该文档比较了不同 {IDF_TARGET_NAME} AT 固件在支持的命令集、硬件配置和模组方面的差异，帮助您确认是否有适合您模组硬件配置的固件。

  .. only:: esp32c3 or esp32c5 or esp32c6 or esp32s2

    - 如果您不确定自己的模组是否可以使用 AT 默认固件，请先阅读 :doc:`../Compile_and_Develop/esp-at_firmware_differences` 文档，该文档比较了不同 {IDF_TARGET_NAME} AT 固件在支持的命令集、硬件配置和模组方面的差异，帮助您确认您的模组硬件配置是否适合使用 AT 默认固件。

  - 如果您想要修改 AT 固件中下面的配置，您可以通过 :doc:`../Compile_and_Develop/tools_at_py` 修改 AT 固件并生成新的 AT 固件。

    .. list::

      - :ref:`at-py-modify-uart`
      - :ref:`at-py-modify-wifi`
      - :ref:`at-py-modify-pki`
      :esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6: - :ref:`at-py-modify-gatts`

特别声明
^^^^^^^^

在使用下面的 AT 固件前，请仔细阅读 :doc:`../disclaimer`，并遵循其中的各项条款和注意事项。

.. only:: esp32

  ESP32-WROOM-32 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-WROOM-32-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V4.1.1.0.zip>`__ （推荐）
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

  ESP32-MINI-1 系列
  ^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-MINI-1-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V4.1.1.0.zip>`__ （推荐）
  - v4.1.0.0 `ESP32-MINI-1-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V4.1.0.0.zip>`__
  - v3.4.0.0 `ESP32-MINI-1-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.4.0.0.zip>`__
  - v3.2.1.0 `ESP32-MINI-1-AT-V3.2.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.2.1.0.zip>`__
  - v3.2.0.0 `ESP32-MINI-1-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-MINI-1-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-MINI-1-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V2.2.0.0.zip>`__

  .. _firmware-esp32-wrover-32-series:

  ESP32-WROVER-32 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  .. note::
    - 此系列不再发布新的 AT 固件，推荐使用 ESP32-MINI-1 系列代替。
    - 由于硬件限制，不推荐使用 ESP32-WROVER-B 模组，请使用其他 WROVER 系列模组。

  - v2.4.0.0 `ESP32-WROVER-32-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-WROVER-32-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-WROVER-32-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-WROVER-32-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.0.0.0.zip>`__
  - v0.10.0.0 `ESP32-WROVER-32-AT-V0.10.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V0.10.0.0.zip>`__

  ESP32-PICO 系列
  ^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-PICO-D4-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V4.1.1.0.zip>`__ （推荐）
  - v4.1.0.0 `ESP32-PICO-D4-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V4.1.0.0.zip>`__
  - v3.4.0.0 `ESP32-PICO-D4-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.4.0.0.zip>`__
  - v3.2.1.0 `ESP32-PICO-D4-AT-V3.2.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.2.1.0.zip>`__
  - v3.2.0.0 `ESP32-PICO-D4-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-PICO-D4-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-PICO-D4-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-PICO-D4-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-PICO-D4-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.0.0.0.zip>`__

  ESP32-SOLO 系列
  ^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-SOLO-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V4.1.1.0.zip>`__ （推荐）
  - v4.1.0.0 `ESP32-SOLO-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V4.1.0.0.zip>`__
  - v3.4.0.0 `ESP32-SOLO-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.4.0.0.zip>`__
  - v3.2.1.0 `ESP32-SOLO-AT-V3.2.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.2.1.0.zip>`__
  - v3.2.0.0 `ESP32-SOLO-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.2.0.0.zip>`__
  - v2.4.0.0 `ESP32-SOLO-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-SOLO-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-SOLO-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-SOLO-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.0.0.0.zip>`__

.. only:: esp32c2

  ESP32-C2-2MB-G2 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  .. note::
    - 此系列不再发布新的 AT 固件，推荐使用兼容的 ESP32-C2-2MB 系列代替。

  下面固件仅适用于 {IDF_TARGET_NAME} ECO4 (Rev v2.0) 系列芯片。

  - v3.3.1.0 `ESP32-C2-2MB-G2-AT-V3.3.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-G2-AT-V3.3.1.0.zip>`__

  ESP32-C2-4MB-G2 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  下面固件仅适用于 {IDF_TARGET_NAME} ECO4 (Rev v2.0) 系列芯片。

  - v4.1.1.0 `ESP32-C2-4MB-G2-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V4.1.1.0.zip>`__ （推荐）
  - v4.1.0.0 `ESP32-C2-4MB-G2-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V4.1.0.0.zip>`__
  - v3.3.1.0 `ESP32-C2-4MB-G2-AT-V3.3.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-G2-AT-V3.3.1.0.zip>`__

  ESP32-C2-2MB 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  下面固件适用于 {IDF_TARGET_NAME} ECO1 (Rev v1.0) ~ ECO4 (Rev v2.0) 系列芯片（包括 ECO1 和 ECO4）。

  - v4.1.1.0 `ESP32-C2-2MB-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V4.1.1.0.zip>`__ （推荐）
  - v4.1.0.0 `ESP32-C2-2MB-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V4.1.0.0.zip>`__

  下面固件适用于 {IDF_TARGET_NAME} ECO1 (Rev v1.0) 和 ECO2 (Rev v1.1) 系列芯片。

  - v3.3.0.0 `ESP32-C2-2MB-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.3.0.0.zip>`__
  - v3.1.0.0 `ESP32-C2-2MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.1.0.0.zip>`__
  - v3.0.0.0 `ESP32-C2-2MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.0.0.0.zip>`__

  ESP32-C2-4MB 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  .. note::
    - 如果您的硬件是 ECO4 (Rev v2.0) 系列芯片，推荐使用 ESP32-C2-4MB-G2 系列代替。

  下面固件适用于 {IDF_TARGET_NAME} ECO1 (Rev v1.0) ~ ECO4 (Rev v2.0) 系列芯片（包括 ECO1 和 ECO4）。

  - v4.1.1.0 `ESP32-C2-4MB-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V4.1.1.0.zip>`__ （推荐）
  - v4.1.0.0 `ESP32-C2-4MB-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V4.1.0.0.zip>`__

  下面固件适用于 {IDF_TARGET_NAME} ECO1 (Rev v1.0) 和 ECO2 (Rev v1.1) 系列芯片。

  - v3.3.0.0 `ESP32-C2-4MB-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.3.0.0.zip>`__
  - v3.1.0.0 `ESP32-C2-4MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.1.0.0.zip>`__
  - v3.0.0.0 `ESP32-C2-4MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.0.0.0.zip>`__

.. only:: esp32c3

  ESP32-C3-MINI-1 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  下面固件适用于 {IDF_TARGET_NAME} ECO0 (Rev v0.0) ~ ECO7 (Rev v1.1) 系列芯片（包括 ECO0 和 ECO7）。

  - v4.1.1.0 `ESP32-C3-MINI-1-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V4.1.1.0.zip>`__ （推荐）
  - v4.1.0.0 `ESP32-C3-MINI-1-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V4.1.0.0.zip>`__
  - v3.3.0.0 `ESP32-C3-MINI-1-AT-V3.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V3.3.0.0.zip>`__

  下面固件适用于 {IDF_TARGET_NAME} ECO0 (Rev v0.0) ~ ECO4 (Rev v0.4) 系列芯片（包括 ECO0 和 ECO4）。

  - v3.2.0.0 `ESP32-C3-MINI-1-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V3.2.0.0.zip>`__
  - v2.4.2.0 `ESP32-C3-MINI-1-AT-V2.4.2.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.2.0.zip>`__
  - v2.4.1.0 `ESP32-C3-MINI-1-AT-V2.4.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.1.0.zip>`__
  - v2.4.0.0 `ESP32-C3-MINI-1-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.0.0.zip>`__
  - v2.3.0.0 `ESP32-C3-MINI-1-AT-V2.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.3.0.0.zip>`__
  - v2.2.0.0 `ESP32-C3-MINI-1-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.2.0.0.zip>`__

.. only:: esp32c5

  ESP32-C5-4MB 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  计划在 2025 年 10 月发布首版 ESP-AT 固件。

.. only:: esp32c6

  ESP32-C6-4MB 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-C6-4MB-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.1.1.0.zip>`__ （推荐）
  - v4.1.0.0 `ESP32-C6-4MB-AT-V4.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.1.0.0.zip>`__
  - v4.0.0.0 `ESP32-C6-4MB-AT-V4.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.0.0.0.zip>`__

.. only:: esp32s2

  ESP32-S2-MINI 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  - v4.1.1.0 `ESP32-S2-MINI-AT-V4.1.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32s2/ESP32-S2-MINI/ESP32-S2-MINI-AT-V4.1.1.0.zip>`__ （推荐）
  - v3.4.0.0 `ESP32-S2-MINI-AT-V3.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32s2/ESP32-S2-MINI/ESP32-S2-MINI-AT-V3.4.0.0.zip>`__

订阅 AT 版本发布
^^^^^^^^^^^^^^^^^^^^^^

请参考 :ref:`sub-at-release` 文档订阅我们的版本发布通知，及时获取最新版本的发布情况。
