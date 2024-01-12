发布的固件
===========

:link_to_translation:`en:[English]`

推荐下载最新版本的固件。目前，乐鑫只发布了以下 {IDF_TARGET_NAME} 系列模组的 AT 固件。

.. note::

  .. only:: esp32c3

    - 当前的 {IDF_TARGET_NAME} AT 固件支持 {IDF_TARGET_NAME} ECO0 (Rev v0.0) ~ ECO4 (Rev v0.4) 系列芯片（包括 ECO0 和 ECO4），暂不支持 {IDF_TARGET_NAME} 的 ECO6 (Rev v1.0)、ECO7 (Rev v1.1) 以及之后的芯片。

  - 如果您的模组没有发布的固件，可以使用相同硬件配置的模组的固件（点击 :doc:`../Compile_and_Develop/esp-at_firmware_differences` 查看与您的模组硬件配置相同的固件），或者如果您需要修改下面的配置，则可以通过 :doc:`../Compile_and_Develop/tools_at_py` 修改发布的固件，为您的模组生成新的固件。

    .. list::

      - :ref:`at-py-modify-uart`
      - :ref:`at-py-modify-wifi`
      - :ref:`at-py-modify-pki`
      :esp32 or esp32c3 or esp32c6: - :ref:`at-py-modify-gatts`

.. only:: esp32

  ESP32-WROOM-32 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  - v3.2.0.0 `ESP32-WROOM-32-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROOM-32/ESP32-WROOM-32-AT-V3.2.0.0.zip>`__ （推荐）
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

  - v3.2.0.0 `ESP32-MINI-1-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V3.2.0.0.zip>`__ （推荐）
  - v2.4.0.0 `ESP32-MINI-1-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-MINI-1-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-MINI-1/ESP32-MINI-1-AT-V2.2.0.0.zip>`__

  .. _firmware-esp32-wrover-32-series:

  ESP32-WROVER-32 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  由于硬件限制，不推荐使用 ESP32-WROVER-B 模组，请使用其他 WROVER 系列模组。

  - v2.4.0.0 `ESP32-WROVER-32-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.4.0.0.zip>`__ （推荐）
  - v2.2.0.0 `ESP32-WROVER-32-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-WROVER-32-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-WROVER-32-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V2.0.0.0.zip>`__
  - v0.10.0.0 `ESP32-WROVER-32-AT-V0.10.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-WROVER-32/ESP32-WROVER-32-AT-V0.10.0.0.zip>`__

  ESP32-PICO 系列
  ^^^^^^^^^^^^^^^^^

  - v3.2.0.0 `ESP32-PICO-D4-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V3.2.0.0.zip>`__ （推荐）
  - v2.4.0.0 `ESP32-PICO-D4-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-PICO-D4-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-PICO-D4-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-PICO-D4-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-PICO-D4/ESP32-PICO-D4-AT-V2.0.0.0.zip>`__

  ESP32-SOLO 系列
  ^^^^^^^^^^^^^^^^^

  - v3.2.0.0 `ESP32-SOLO-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V3.2.0.0.zip>`__ （推荐）
  - v2.4.0.0 `ESP32-SOLO-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.4.0.0.zip>`__
  - v2.2.0.0 `ESP32-SOLO-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.2.0.0.zip>`__
  - v2.1.0.0 `ESP32-SOLO-AT-V2.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.1.0.0.zip>`__
  - v2.0.0.0 `ESP32-SOLO-AT-V2.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32/ESP32-SOLO/ESP32-SOLO-AT-V2.0.0.0.zip>`__

.. only:: esp32c2

  ESP32-C2 2MB 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  - v3.1.0.0 `ESP32-C2-2MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.1.0.0.zip>`__ （推荐）
  - v3.0.0.0 `ESP32-C2-2MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-2MB-AT-V3.0.0.0.zip>`__

  ESP32-C2 4MB 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  - v3.1.0.0 `ESP32-C2-4MB-AT-V3.1.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.1.0.0.zip>`__ （推荐）
  - v3.0.0.0 `ESP32-C2-4MB-AT-V3.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c2/ESP32-C2-4MB-AT-V3.0.0.0.zip>`__

.. only:: esp32c3

  ESP32-C3-MINI-1 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  - v3.2.0.0 `ESP32-C3-MINI-1-AT-V3.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V3.2.0.0.zip>`__ （推荐）
  - v2.4.2.0 `ESP32-C3-MINI-1-AT-V2.4.2.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.2.0.zip>`__
  - v2.4.1.0 `ESP32-C3-MINI-1-AT-V2.4.1.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.1.0.zip>`__
  - v2.4.0.0 `ESP32-C3-MINI-1-AT-V2.4.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.4.0.0.zip>`__
  - v2.3.0.0 `ESP32-C3-MINI-1-AT-V2.3.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.3.0.0.zip>`__
  - v2.2.0.0 `ESP32-C3-MINI-1-AT-V2.2.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c3/ESP32-C3-MINI-1-AT-V2.2.0.0.zip>`__

.. only:: esp32c6

  ESP32-C6 4MB 系列
  ^^^^^^^^^^^^^^^^^^^^^^

  - v4.0.0.0 `ESP32-C6-4MB-AT-V4.0.0.0.zip <https://dl.espressif.com/esp-at/firmwares/esp32c6/ESP32-C6-4MB-AT-V4.0.0.0.zip>`__ （推荐）
