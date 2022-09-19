Index of Abbreviations
=======================

.. glossary::

    A2DP
      Advanced Audio Distribution Profile
    
      高级音频分发框架
    
    ADC
      Analog-to-Digital Converter
    
      模拟数字转换器
    
    ALPN
      Application Layer Protocol Negotiation
    
      应用层协议协商

    AT
      AT stands for "attention". 

      AT 是 attention 的缩写。
    
    AT command port
      The port that is used to send AT commands and receive responses. More details are in the :term:`AT port` introduction.

    AT 命令端口
      也称为 AT 命令口，用于发送 AT 命令和接收响应的端口。更多介绍请参考 :term:`AT 端口`。

    AT log port
      The port that is used to output log. More details are in the :term:`AT port` introduction.

    AT 日志端口
      也称为 AT 日志口，用于输出 AT 日志的端口。更多介绍请参考 :term:`AT 端口`。

    AT port
      AT port is the general name of AT log port (that is used to output log) and AT command port (that is used to send AT commands and receive responses). Please refer to :doc:`Get_Started/Hardware_connection` for default AT port pins and :doc:`Compile_and_Develop/How_to_set_AT_port_pin` for how to customize them.

    AT 端口
      AT 端口是 AT 日志端口（用于输出日志）和 AT 命令端口（用于发送 AT 命令和接收响应）的总称。请参考 :doc:`Get_Started/Hardware_connection` 了解默认的 AT 端口管脚，参考 :doc:`Compile_and_Develop/How_to_set_AT_port_pin` 了解如何自定义 AT 端口管脚。

    Bluetooth LE
      Bluetooth Low Energy
    
      低功耗蓝牙
    
    BluFi
      Wi-Fi network configuration function via Bluetooth channel
    
      BluFi 是一款基于蓝牙通道的 Wi-Fi 网络配置功能

    Command Mode
      Default operating mode of AT. In the command mode, any character received by the AT command port will be treated as an AT command, and AT returns the command execution result to the AT command port.
      AT enters :term:`Data Mode` from :term:`Command Mode` in the following cases.

      .. list::

        * After sending the :ref:`AT+CIPSEND <cmd-SEND>` set command successfully and returns `>`.
        * After sending the :ref:`AT+CIPSEND <cmd-SEND>` execute command successfully and returns `>`.
        * After sending the :ref:`AT+CIPSENDL <cmd-SENDL>` set command successfully and returns `>`.
        * After sending the :ref:`AT+CIPSENDEX <cmd-SENDEX>` set command successfully and returns `>`.
        * After sending the :ref:`AT+SAVETRANSLINK <cmd-SAVET>` set command successfully and sending the :ref:`AT+RST <cmd-RST>` command and restart the module.
        :esp32: * After sending the :ref:`AT+BTSPPSEND <cmd-BTSPPSEND>` execute command successfully and returns `>`.
        :esp32 or esp32c3: * After sending the :ref:`AT+BLESPP <cmd-BLESPP>` execute command successfully and returns `>`.

      In the data mode, send the :ref:`+++ <cmd-PLUS>` command, AT will exit from :term:`Data Mode` and enter the :term:`Command Mode`.

    命令模式
      AT 的默认工作模式。在命令模式下，AT 命令端口收到的任何字符都会被当作 AT 命令进行处理，同时 AT 会在命令端口回复命令执行结果。
      AT 在下列情况下，会从 :term:`命令模式` 进入 :term:`数据模式`。

      .. list::

        * 发送 :ref:`AT+CIPSEND <cmd-SEND>` 设置命令成功，回复 `>` 之后
        * 发送 :ref:`AT+CIPSEND <cmd-SEND>` 执行命令成功，回复 `>` 之后
        * 发送 :ref:`AT+CIPSENDL <cmd-SENDL>` 设置命令成功，回复 `>` 之后
        * 发送 :ref:`AT+CIPSENDEX <cmd-SENDEX>` 设置命令成功，回复 `>` 之后
        * 发送 :ref:`AT+SAVETRANSLINK <cmd-SAVET>` 设置命令成功，再发送 (:ref:`AT+RST <cmd-RST>`) 命令，模组重启之后
        :esp32: * 发送 :ref:`AT+BTSPPSEND <cmd-BTSPPSEND>` 执行命令成功，回复 `>` 之后
        :esp32 or esp32c3: * 发送 :ref:`AT+BLESPP <cmd-BLESPP>` 执行命令成功，回复 `>` 之后

      在数据模式下，发送 :ref:`+++ <cmd-PLUS>` 命令，会从 :term:`数据模式` 退出，进入 :term:`命令模式`。

    Data Mode
      In the data mode, any character received by the AT command port will be treated as data (except for special :ref:`+++ <cmd-PLUS>`) instead of the AT command, and these data will be sent to the opposite end without modification.
      AT enters :term:`Data Mode` from :term:`Command Mode` in the following cases.

      .. list::

        * After sending the :ref:`AT+CIPSEND <cmd-SEND>` set command successfully and returns `>`.
        * After sending the :ref:`AT+CIPSEND <cmd-SEND>` execute command successfully and returns `>`.
        * After sending the :ref:`AT+CIPSENDL <cmd-SENDL>` set command successfully and returns `>`.
        * After sending the :ref:`AT+CIPSENDEX <cmd-SENDEX>` set command successfully and returns `>`.
        * After sending the :ref:`AT+SAVETRANSLINK <cmd-SAVET>` set command successfully and sending the :ref:`AT+RST <cmd-RST>` command and restart the module.
        :esp32: * After sending the :ref:`AT+BTSPPSEND <cmd-BTSPPSEND>` execute command successfully and returns `>`.
        :esp32 or esp32c3: * After sending the :ref:`AT+BLESPP <cmd-BLESPP>` execute command successfully and returns `>`.

      In the data mode, send the :ref:`+++ <cmd-PLUS>` command, AT will exit from :term:`Data Mode` and enter the :term:`Command Mode`.

    数据模式
      在数据模式下，AT 命令端口收到的任何字符都会被当作数据（除了特殊的 :ref:`+++ <cmd-PLUS>`），而不是 AT 命令，这些数据会无修改的发往对端。
      AT 在下列情况下，会从 :term:`命令模式` 进入 :term:`数据模式`。

      .. list::

        * 发送 :ref:`AT+CIPSEND <cmd-SEND>` 设置命令成功，回复 `>` 之后
        * 发送 :ref:`AT+CIPSEND <cmd-SEND>` 执行命令成功，回复 `>` 之后
        * 发送 :ref:`AT+CIPSENDL <cmd-SENDL>` 设置命令成功，回复 `>` 之后
        * 发送 :ref:`AT+CIPSENDEX <cmd-SENDEX>` 设置命令成功，回复 `>` 之后
        * 发送 :ref:`AT+SAVETRANSLINK <cmd-SAVET>` 设置命令成功，再发送 :ref:`AT+RST <cmd-RST>` 命令，模组重启之后
        :esp32: * 发送 :ref:`AT+BTSPPSEND <cmd-BTSPPSEND>` 执行命令成功，回复 `>` 之后
        :esp32 or esp32c3: * 发送 :ref:`AT+BLESPP <cmd-BLESPP>` 执行命令成功，回复 `>` 之后

      在数据模式下，发送 :ref:`+++ <cmd-PLUS>` 命令，会从 :term:`数据模式` 退出，进入 :term:`命令模式`。

    DHCP
      Dynamic Host Configuration Protocol
      
      动态主机配置协议
    
    DNS
      Domain Name System
    
      域名系统
    
    DTIM
      Delivery Traffic Indication Map
    
      延迟传输指示映射 
    
    GATTC
      Generic Attributes client
    
      GATT 客户端
    
    GATTS
      Generic Attributes server
    
      GATT 服务器
    
    HID
      Human Interface Device
    
      人机接口设备
    
    I2C
      Inter-Integrated Circuit
    
      集成电路总线
    
    ICMP
      Intemet Control Message Protocol
    
      因特网控制报文协议
    
    LwIP
      A Lightweight TCP/IP stack

      一个轻量级的 TCP/IP 协议栈

    LWT
      Last Will and Testament
    
      遗嘱
    
    MAC
      Media Access Control
      
      MAC 地址
    
    mDNS
      Multicast Domain Name System
      
      多播 DNS
    
    MSB
      Most Significant Bit
      
      最高有效位
    
    MTU
      maximum transmission unit
      
      最大传输单元
    
    NVS
      Non-Volatile Storage
      
      非易失性存储器
    
    Normal Transmission Mode
      Default Transmission Mode

      In normal transmission mode, users can send AT commands. For examples, users can send MCU data received by AT command port to the opposite end of transmission by :ref:`AT+CIPSEND <cmd-SEND>`; and the data received from the opposite end of transmission will also be returned to MCU through AT command port with additional prompt: `+IPD`.

      During a normal transmission, if the connection breaks, {IDF_TARGET_NAME} will give a prompt and will not attempt to reconnect.

      More details are in :term:`Transmission Mode Shift Diagram`.

    普通传输模式
      默认传输模式

      在普通传输模式下，用户可以发送 AT 命令。
      例如，用户可以通过 :ref:`AT+CIPSEND <cmd-SEND>` 命令，发送 AT 命令口收到的 MCU 数据到传输对端。从传输对端收到的数据，会通过 AT 命令口返回给 MCU，同时会附带 `+IPD` 信息。

      普通传输模式时，如果连接断开，{IDF_TARGET_NAME} 不会重连，并提示连接断开。

      更多介绍请参考 :term:`Transmission Mode Shift Diagram`。

    OWE
      Opportunistic Wireless Encryption. OWE is a Wi-Fi standard which ensures that the communication between each pair of endpoints is protected from other endpoints.

      More details are in `Wikipedia <https://en.wikipedia.org/wiki/Opportunistic_Wireless_Encryption>`__.

      机会性无线加密。OWE 是一种 Wi-Fi 标准，它确保每对端点之间的通信受到保护，不受其他端点的影响。

      更多介绍请参考 `维基百科 <https://en.wikipedia.org/wiki/Opportunistic_Wireless_Encryption>`__。

    Passthrough Mode
      Also called as "Passthrough Sending & Receiving Mode".
      
      In passthrough mode, users cannot send AT commands except special :ref:`+++ <cmd-PLUS>` command. All MCU data received by AT command port will be sent to the opposite end of transmission without any modification; and the data received from the opposite end of transmission will also be returned to MCU through AT command port without any modification.

      During the Wi-Fi passthrough transmission, if the connection breaks, {IDF_TARGET_NAME} (as client) will keep trying to reconnect until :ref:`+++ <cmd-PLUS>` is input to exit the passthrough transmission; {IDF_TARGET_NAME} (as server) will shutdown the old connection and listen new connection until :ref:`+++ <cmd-PLUS>` is input to exit the passthrough transmission.

      More details are in :term:`Transmission Mode Shift Diagram`.

    透传模式
      也称为 “透传发送接收模式”。

      在透传模式下，用户不能发送其它 AT 命令，除了特别的 :ref:`+++ <cmd-PLUS>` 命令。AT 命令口收到的所有的 MCU 数据都将无修改地，发送到传输对端。从传输对端收到的数据也会通过 AT 命令口无修改地，返回给 MCU。

      Wi-Fi 透传模式传输时，如果连接断开，{IDF_TARGET_NAME} 作为客户端时，会不停地尝试重连，此时单独输入 :ref:`+++ <cmd-PLUS>` 退出透传，则停止重连；{IDF_TARGET_NAME} 作为服务器时，会关闭连接同时监听新的连接，此时单独输入 :ref:`+++ <cmd-PLUS>` 退出透传。

      更多介绍请参考 :term:`Transmission Mode Shift Diagram`。

    Transmission Mode Shift Diagram
      .. figure:: ../_static/intro-tt-mode.png
        :align: center
        :alt: Transmission Mode Shift Diagram
        :figclass: align-center
    
        Transmission Mode Shift Diagram

      More details are in the following introduction.

      * :term:`Normal Transmission Mode` (:term:`普通传输模式`)
      * :term:`Passthrough Receiving Mode` (:term:`透传接收模式`)
      * :term:`Passthrough Mode` (:term:`透传模式`)
      * :ref:`AT+CIPMODE <cmd-IPMODE>`
      * :ref:`AT+CIPSEND <cmd-SEND>`
      * :ref:`+++ <cmd-PLUS>`
      * :ref:`AT+SAVETRANSLINK <cmd-SAVET>`

    Passthrough Receiving Mode
      The temporary mode between :term:`Normal Transmission Mode` and :term:`Passthrough Mode`.

      In passthrough receiving mode, AT cannot send any data to the opposite end of transmission; but the data received from the opposite end of transmission can be returned to MCU through AT command port without any modification. More details are in :term:`Transmission Mode Shift Diagram`.

    透传接收模式
      在 :term:`普通传输模式` 和 :term:`透传模式` 之间的一个临时模式。

      在透传接收模式，AT 不能发送数据到传输对端；但 AT 可以收到来自传输对端的数据，通过 AT 命令口无修改地返回给 MCU。更多介绍请参考 :term:`Transmission Mode Shift Diagram`。

    PBC
      Push Button Configuration
      
      按钮配置
    
    PCI Authentication
      Payment Card Industry Authentication. In ESP-AT project, it refers to all Wi-Fi authentication modes except OPEN and WEP.
      
      PCI 认证，在 ESP-AT 工程中指的是除 OPEN 和 WEP 以外的 Wi-Fi 认证模式。

    PKI
      A public key infrastructure (PKI) is a set of roles, policies, hardware, software and procedures needed to create, manage, distribute, use, store and revoke digital certificates and manage public-key encryption.

      More details are in `Public Key Infrastructure <https://en.wikipedia.org/wiki/Public_key_infrastructure>`__.

      公开密钥基础建设。公开密钥基础建设（PKI）是一组由硬件、软件、参与者、管理政策与流程组成的基础架构，其目的在于创造、管理、分配、使用、存储以及撤销数字证书。

      更多介绍请参考 `公开密钥基础建设 <https://zh.wikipedia.org/zh-cn/%E5%85%AC%E9%96%8B%E9%87%91%E9%91%B0%E5%9F%BA%E7%A4%8E%E5%BB%BA%E8%A8%AD>`__。

    PLCP
      Physical Layer Convergence Procedure
      
      PLCP 协议，即物理层会聚协议
    
    PMF
      protected management frame
      
      受保护的管理帧
    
    PSK
      Pre-shared Key
      
      预共享密钥
    
    PWM
      Pulse-Width Modulation
      
      脉冲宽度调制
    
    QoS
      Quality of Service
      
      服务质量
    
    RTC
      Real Time Controller. A group of circuits in SoC that keeps working in any chip mode and at any time.
      
      实时控制器，为 SoC 中的一组电路，在任何芯片模式下都能随时保持工作。
    
    SMP
      Security Manager Protocol
      
      安全管理协议
    
    SNI
      Server Name Indication
      
      服务器名称指示
    
    SNTP
      Simple Network Time Protocol
      
      简单网络时间协议
    
    SPI
      Serial Peripheral Interface
      
      串行外设接口
    
    SPP
      Serial Port Profile
      
      SPP 协议，即串口协议
    
    SSL
      Secure Sockets Layer
      
      SSL 协议，即安全套接字协议
    
    TLS
      Transport Layer Security
      
      TLS 协议，即传输层安全性协议 
    
    URC
      Unsolicited Result Code
      
      非请求结果码，一般为模组给 MCU 的串口返回
    
    UTC
      Coordinated Universal Time
      
      协调世界时
    
    UUID
      universally unique identifier
      
      通用唯一识别码
    
    WEP
      Wired-Equivalent Privacy
      
      WEP 加密方式，即有线等效加密
    
    WPA
      Wi-Fi Protected Access
      
      Wi-Fi 保护访问
    
    WPA2
      Wi-Fi Protected Access II
      
      Wi-Fi 保护访问 II
    
    WPS
      Wi-Fi Protected Setup
      
      Wi-Fi 保护设置
