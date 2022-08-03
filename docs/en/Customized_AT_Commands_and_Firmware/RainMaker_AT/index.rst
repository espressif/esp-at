******************************************
RainMaker AT Commands and Firmware
******************************************

.. toctree::
   :maxdepth: 1
   
   RainMaker AT Command Set <RainMaker_AT_Command_Set>
   RainMaker AT Examples <RainMaker_AT_Examples>
   Index of Abbreviations <index_of_abbreviations>

.. _rm-at-messages:

RainMaker AT Messages
======================

.. _rm-at-messages-report:

- ESP-AT RainMaker Message Reports (active)

  ESP-AT will report important RainMaker state changes or messages in the system.

  .. list-table:: ESP-AT Message Report
     :header-rows: 1
     :widths: 60 60

     * - ESP-AT RainMaker Message Report
       - Description
     * - General AT messages
       - Please refer to :ref:`ESP-AT Message Reports (active) <at-messages-report>`
     * - +RMRECV:<src_name>,<device_name>,<param_name>:<param_value>
       - ESP-AT received messages from the RainMaker cloud, and ``<param_value>`` is a string
     * - +RMCONNECTED
       - RainMaker device connected to the cloud
     * - +RMDISCONNECTED
       - RainMaker device passively disconnected from the cloud
     * - +RMRESET
       - RainMaker device received reset message. Host MCU should actively execute the command :ref:`AT+RST <cmd-RST>` to restart the module after receiving the message
     * - +RMREBOOT
       - RainMaker device received reboot message. Host MCU should actively execute the command :ref:`AT+RST <cmd-RST>` to restart the module after receiving the message
     * - +RMTIMEZONE
       - RainMaker device received timezone message
     * - +RMMAPPINGDONE
       - RainMaker device completed user-node mapping
