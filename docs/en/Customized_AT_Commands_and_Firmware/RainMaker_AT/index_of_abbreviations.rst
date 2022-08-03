Index of Abbreviations
=======================

.. glossary::

    RainMaker Normal Transmission Mode
      Default Transmission Mode

      In RainMaker Normal Transmission Mode, you must first add parameters to the device. ESP-AT will convert the parameters to the RainMaker data model and communicate with the cloud in string form (JSON format). In this mode, you can operate multiple parameters of the device.

    RainMaker 普通传输模式
      默认传输模式

      在普通传输模式下，您必须先在设备下添加参数，ESP-AT 会将参数转换为 RainMaker 数据模型后以字符串形式（JSON 格式）和云端通信。在该模式下，您可以对设备下的多个参数进行操作。

    RainMaker Passthrough Mode
      Also called as "RainMaker Passthrough Sending & Receiving Mode".

      RainMaker Passthrough Mode only supports one parameter under the device (excluding default parameters). The topic to publish is ``node/<node_id>/params/local``. ESP-AT will automatically pushes the data sent by the MCU to the RainMaker cloud in string form (JSON format). The topic to subscribe to is ``node/<node_id>/params/remote``. ESP-AT will automatically parses the data received from the RainMaker cloud and sends the parsed data (string format) to the MCU.

      In RainMaker Passthrough Mode, other AT commands can not be sent except the special :ref:`+++ <cmd-PLUS>` command.

      In RainMaker Passthrough Mode, the scheduling service unavailable.

    RainMaker 透传模式
      也称为 “RainMaker 透传发送接收模式”。

      RainMaker 透传模式仅支持设备中存在一个参数（不包括默认参数）。发布主题为 ``node/<node_id>/params/local``。ESP-AT 会自动将 MCU 发送的数据以字符串形式（JSON 格式）推送 RainMaker 云端。订阅主题为 ``node/<node_id>/params/remote``。ESP-AT 会自动将从 RainMaker 云端接收到的数据进行解析，并将解析后的数据（字符串格式）发送给 MCU。

      在 RainMaker 透传模式下，除了特殊命令 :ref:`+++ <cmd-PLUS>`，无法发送其他 AT 命令。

      在 RainMaker 透传模式下，定时服务不可用。
