ESP-AT 不同版本命令集支持对比
===================================

:link_to_translation:`en:[English]`

本文档主要列举了旧版 NONOS-AT 向新版 ESP-AT 迁移时，需要注意的命令上的差异。

- NONOS-AT：**不推荐** 使用的 ESP8266 AT 版本，早期用于 ESP8266 系列芯片，基于 ESP8266_NONOS_SDK 开发，已停止更新。
- ESP-AT：**推荐** 使用的版本，支持多个芯片平台，与 NONOS-AT 相比，支持更多种类的命令，如 Bluetooth 命令、Bluetooth LE 命令、以太网命令、驱动命令等。更多有关 ESP-AT 的信息请参阅 :doc:`../Get_Started/What_is_ESP-AT` 和 :doc:`../AT_Binary_Lists/index`。

.. 注意::
  下表列出了所有的 NONOS-AT 命令，以及它们在 ESP-AT 中的支持情况。下表并不包含所有的 ESP-AT 命令，若想了解所有 ESP-AT 命令，请参考 :doc:`AT 命令集 <index>`。

  下表中第一列的命令链接均指向 ESP-AT 命令。

.. list-table:: ESP-AT 不同版本命令集支持对比表
   :header-rows: 1
   :widths: 25 30 15 30

   * - 命令
     - 简介
     - NONOS-AT
     - ESP-AT
   * - :ref:`AT <cmd-AT>`
     - 测试 AT 启动
     - ✅
     - ✅
   * - :ref:`AT+RST <cmd-RST>`
     - 重启模块
     - ✅
     - ✅
   * - :ref:`AT+GMR <cmd-GMR>`
     - 查询版本信息
     - ✅
     - ✅
   * - :ref:`AT+GSLP <cmd-GSLP>`
     - 进入 deep-sleep 模式
     - ✅
     - ✅
   * - :ref:`ATE <cmd-ATE>`
     - 开关回显功能
     - ✅
     - ✅
   * - :ref:`AT+RESTORE <cmd-RESTORE>`
     - 恢复出厂设置
     - ✅
     - ✅
   * - :ref:`AT+UART_CUR <cmd-UARTC>`
     - 配置 UART，不保存到 flash
     - ✅
     - ✅
   * - :ref:`AT+UART_DEF <cmd-UARTD>`
     - 配置 UART，保存到 flash
     - ✅
     - ✅
   * - :ref:`AT+SLEEP <cmd-SLEEP>`
     - 设置 sleep 模式
     - 
       | ✅
       | 1: light sleep
       | 2: modem sleep
     - | ESP8266 ✅
       | ESP32 ✅
       | ESP32-S2 ❌
       | 注释 [#one]_
   * - AT+WAKEUPGPIO
     - 配置 GPIO 唤醒 light sleep
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+SLEEPWKCFG <cmd-WKCFG>`
   * - :ref:`AT+RFPOWER <cmd-RFPOWER>`
     - 设置 RF TX Power
     - | ✅
       | 范围：[0,82]
       | 单位：0.25 dBm
     - ✅ [#two]_
   * - AT+RFVDD
     - 根据 VDD33 设置 RF TX Power
     - ✅
     - ❌
   * - :ref:`AT+SYSRAM <cmd-SYSRAM>`
     - 查询系统当前剩余内存
     - ✅
     - ✅ 新增最小峰值内存参数
   * - AT+SYSADC
     - 查询 ADC 值
     - ✅
     - ❌
   * - AT+SYSIOSETCFG
     - 设置 IO 工作模式
     - ✅
     - ❌
   * - AT+SYSIOGETCFG
     - 查询 IO 工作模式
     - ✅
     - ❌
   * - AT+SYSGPIODIR
     - 设置 GPIO 工作为输入或输出
     - ✅
     - ❌
   * - AT+SYSGPIOWRITE
     - 设置 GPIO 的输出电平
     - ✅
     - ❌
   * - AT+SYSGPIOREAD
     - 读取 GPIO 的电平状态
     - ✅
     - ❌
   * - :ref:`AT+SYSMSG <cmd-SYSMSG>`
     - 设置系统消息
     - ❌
     - ✅
   * - AT+SYSMSG_CUR
     - 设置当前系统消息，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+SYSMSG <cmd-SYSMSG>`
   * - AT+SYSMSG_DEF
     - 设置默认系统消息，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+SYSMSG <cmd-SYSMSG>`
   * - :ref:`AT+CWMODE <cmd-MODE>`
     - 设置 Wi-Fi 模式
     - ❌
     - ✅ 新增切换模式自动连接
   * - AT+CWMODE_CUR
     - 设置当前 Wi-Fi 模式，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWMODE <cmd-MODE>`
   * - AT+CWMODE_DEF
     - 设置默认 Wi-Fi 模式，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWMODE <cmd-MODE>`
   * - :ref:`AT+CWJAP <cmd-JAP>`
     - 连接 AP
     - ✅
     - ✅ 新增更多功能
   * - AT+CWJAP_CUR
     - 连接 AP，参数不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWJAP <cmd-JAP>`
   * - AT+CWJAP_DEF
     - 连接 AP，参数保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWJAP <cmd-JAP>`
   * - :ref:`AT+CWLAPOPT <cmd-LAPOPT>`
     - 设置 CWLAP 命令的属性
     - ✅
     - ✅ 新增更多功能
   * - :ref:`AT+CWLAP <cmd-LAP>`
     - 扫描当前可⽤ AP
     - ✅
     - ✅ 回复不同
   * - :ref:`AT+CWQAP <cmd-QAP>`
     - 断开与 AP 连接
     - ✅
     - ✅
   * - :ref:`AT+CWSAP <cmd-SAP>`
     - 配置 softAP 参数
     - ❌
     - ✅
   * - AT+CWSAP_CUR
     - 配置 softAP 参数，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWSAP <cmd-SAP>`
   * - AT+CWSAP_DEF
     - 配置 softAP 参数，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWSAP <cmd-SAP>`
   * - :ref:`AT+CWLIF <cmd-LIF>`
     - 查询连接到 softAP 的 Station 信息
     - ✅
     - ✅
   * - :ref:`AT+CWDHCP <cmd-DHCP>`
     - 设置 DHCP
     - ❌
     - ✅
   * - AT+CWDHCP_CUR
     - 设置 DHCP，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWDHCP <cmd-DHCP>`
   * - AT+CWDHCP_DEF
     - 设置 DHCP，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWDHCP <cmd-DHCP>`
   * - :ref:`AT+CWDHCPS <cmd-DHCPS>`
     - 设置 softAP DHCP 分配的 IP 范围
     - ❌
     - ✅
   * - AT+CWDHCPS_CUR
     - 设置 softAP DHCP 分配的 IP 范围，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWDHCPS <cmd-DHCPS>`
   * - AT+CWDHCPS_DEF
     - 设置 softAP DHCP 分配的 IP 范围，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWDHCPS <cmd-DHCPS>`
   * - :ref:`AT+CWAUTOCONN <cmd-AUTOC>`
     - 上电是否自动连接 AP
     - ✅
     - ✅
   * - :ref:`AT+CIPSTAMAC <cmd-STAMAC>`
     - 设置 Station 接⼝的 MAC 地址
     - ❌
     - ✅
   * - AT+CIPSTAMAC_CUR
     - 设置 Station 接口 MAC 地址，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPSTAMAC <cmd-STAMAC>`
   * - AT+CIPSTAMAC_DEF
     - 设置 Station 接口 MAC 地址，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPSTAMAC <cmd-STAMAC>`
   * - :ref:`AT+CIPAPMAC <cmd-APMAC>`
     - 设置 softAP 的 MAC 地址
     - ❌
     - ✅
   * - AT+CIPAPMAC_CUR
     - 设置 softAP 的 MAC 地址，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPAPMAC <cmd-APMAC>`
   * - AT+CIPAPMAC_DEF
     - 设置 softAP 的 MAC 地址，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPAPMAC <cmd-APMAC>`
   * - :ref:`AT+CIPSTA <cmd-IPSTA>`
     - 设置 Station 的 IP 地址
     - ❌
     - ✅
   * - AT+CIPSTA_CUR
     - 设置 Station 的 IP 地址，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPSTA <cmd-IPSTA>`
   * - AT+CIPSTA_DEF
     - 设置 Station 的 IP 地址，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPSTA <cmd-IPSTA>`
   * - :ref:`AT+CIPAP <cmd-IPAP>`
     - 设置 softAP 的 IP 地址
     - ❌
     - ✅
   * - AT+CIPAP_CUR
     - 设置 softAP 的 IP 地址，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPAP <cmd-IPAP>`
   * - AT+CIPAP_DEF
     - 设置 softAP 的 IP 地址，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPAP <cmd-IPAP>`
   * - :ref:`AT+CWSTARTSMART <cmd-STARTS>`
     - 开启 SmartConfig
     - ✅
     - ✅
   * - :ref:`AT+CWSTOPSMART <cmd-STOPS>`
     - 停止 SmartConfig
     - ✅
     - ✅
   * - AT+CWSTARTDISCOVER
     - 开启可被局域⽹网内微信探测模式
     - ✅
     - ❌
   * - AT+CWSTOPDISCOVER
     - 关闭可被局域⽹网内微信探测模式
     - ✅
     - ❌
   * - :ref:`AT+WPS <cmd-WPS>`
     - 设置 WPS 功能
     - ✅
     - ✅
   * - :ref:`AT+MDNS <cmd-MDNS>`
     - 设置 mDNS 功能
     - ✅
     - ✅
   * - :ref:`AT+CWJEAP <cmd-JEAP>`
     - 连接企业级加密路由器
     - ❌
     -
       | ESP8266 ❌
       | ESP32 ✅
       | ESP32-S2 ❌
   * - AT+CWJEAP_CUR
     - 连接企业级加密路由器，不保存到 flash
     - ✅
     - ❌
   * - AT+CWJEAP_DEF
     - 连接企业级加密路由器，保存到 flash
     - ✅
     - ❌
   * - :ref:`AT+CWHOSTNAME <cmd-HOSTNAME>`
     - 设置 Station 的主机名称
     - ✅
     - ✅
   * - :ref:`AT+CWCOUNTRY <cmd-COUNTRY>`
     - 设置 Wi-Fi 国家码
     - ❌
     - ✅
   * - AT+CWCOUNTRY_CUR
     - 设置 Wi-Fi 国家码，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWCOUNTRY <cmd-COUNTRY>`
   * - AT+CWCOUNTRY_DEF
     - 设置 Wi-Fi 国家码，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CWCOUNTRY <cmd-COUNTRY>`
   * - :ref:`AT+CIPSTATUS <cmd-STATUS>`
     - 查询⽹网络连接信息
     - ✅
     - ✅
   * - :ref:`AT+CIPDOMAIN <cmd-DOMAIN>`
     - 域名解析功能
     - ✅
     - ✅
   * - :ref:`AT+CIPSTART <cmd-START>`
     - 建立 TCP/UDP/SSL 连接
     - ✅
     - ✅
   * - AT+CIPSSLSIZE
     - 设置 SSL Buffer size
     - ✅
     - ❌
   * - :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
     - 配置 SSL 连接属性
     - ✅
     - ✅ 参数不同
   * - :ref:`AT+CIPSEND <cmd-SEND>`
     - 发送数据
     - ✅
     - ✅
   * - :ref:`AT+CIPSENDEX <cmd-SENDEX>`
     - 发送长度为 length 数据，或在长度内 \0 结尾的数据
     - ✅
     - ✅
   * - AT+CIPSENDBUF
     - 数据写入 TCP 发包缓存
     - ✅
     - ❌ [#three]_
   * - AT+CIPBUFRESET
     - 重新计数
     - ✅
     - ❌ [#three]_
   * - AT+CIPBUFSTATUS
     - 查询 TCP 发包缓存的状态
     - ✅
     - ❌ [#three]_
   * - AT+CIPCHECKSEQ
     - 查询写入 TCP 发包缓存的某包是否发送成功
     - ✅
     - ❌ [#three]_
   * - AT+CIPCLOSEMODE
     - 设置 TCP 连接的断开方式
     - ✅
     - ❌ [#three]_
   * - :ref:`AT+CIPCLOSE <cmd-CLOSE>`
     - 关闭 TCP/UDP/SSL 传输
     - ✅
     - ✅
   * - :ref:`AT+CIFSR <cmd-IFSR>`
     - 查询本地 IP 地址
     - ✅
     - ✅
   * - :ref:`AT+CIPMUX <cmd-MUX>`
     - 设置多连接
     - ✅
     - ✅
   * - :ref:`AT+CIPSERVER <cmd-SERVER>`
     - 建立 TCP/SSL 服务器
     - ✅ 不支持 SSL server
     -
       ✅ ESP32 和 ESP32-S2 支持 SSL server，ESP8266 不支持 SSL server
   * - :ref:`AT+CIPSERVERMAXCONN <cmd-SERVERMAX>`
     - 设置服务器允许建立的最⼤连接数
     - ✅
     - ✅
   * - :ref:`AT+CIPMODE <cmd-IPMODE>`
     - 设置传输模式
     - ✅
     - ✅
   * - :ref:`AT+SAVETRANSLINK <cmd-SAVET>`
     - 设置开机 :term:`透传模式` 信息
     - ✅
     - ✅
   * - :ref:`AT+CIPSTO <cmd-STO>`
     - 设置 TCP 服务器器超时时间
     - ✅
     - ✅
   * - :ref:`AT+PING <cmd-CIPPING>`
     - Ping 功能
     - ✅
     - ✅
   * - :ref:`AT+CIUPDATE <cmd-UPDATE>`
     - 通过 Wi-Fi 升级固件
     - ✅
     - ✅ 支持更多参数
   * - :ref:`AT+CIPDINFO <cmd-IPDINFO>`
     - 接收网络数据时是否提示对端 IP 和端⼝
     - ✅
     - ✅
   * - :ref:`AT+CIPRECVMODE <cmd-CIPRECVMODE>`
     - 设置 TCP 连接的数据接收方式
     - ✅
     - ✅
   * - :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>`
     - 被动接收模式时，读取缓存的 TCP 数据
     - ✅
     - ✅ 回复有差异
   * - :ref:`AT+CIPRECVLEN <cmd-CIPRECVLEN>`
     - 被动接收模式时，查询缓存 TCP 数据的长度
     - ✅
     - ✅
   * - :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>`
     - 设置时域和 SNTP 服务器
     - ✅
     - ✅ 支持更多功能
   * - :ref:`AT+CIPSNTPTIME <cmd-SNTPT>`
     - 查询 SNTP 时间
     - ✅
     - ✅
   * - :ref:`AT+CIPDNS <cmd-DNS>`
     - ⾃定义 DNS 服务器
     - ❌
     - ✅
   * - AT+CIPDNS_CUR
     - 自定义 DNS 服务器，不保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPDNS <cmd-DNS>`
   * - AT+CIPDNS_DEF
     - 自定义 DNS 服务器，保存到 flash
     - ✅
     - | ❌ [#three]_ 相似命令参考 :ref:`AT+CIPDNS <cmd-DNS>`
   * - :ref:`AT+SYSFLASH <cmd-SYSFLASH>`
     - 读写 flash 用户分区
     - ❌
     - ✅

提示：点击脚注序号即可返回/跳转到上方表格的标注处。

.. [#one] 新版 ESP-AT 中的 AT+SLEEP

       * ESP8266 and ESP32 ✅
       
         * 1：modem sleep by DTIM
         * 2：light sleep
         * 3：modem sleep by listen interval
       
       * ESP32-S2 ❌
.. [#two] 新版 ESP-AT 中的 AT+RFPOWER

       * ESP8266 ✅，范围：[40,82]，单位：0.25 dBm
       * ESP32 ✅，范围：[40,78]，单位：0.25 dBm，且支持 Bluetooth LE
       * ESP32-S2 ✅，范围：[40,78]，单位：0.25 dBm

.. [#three] 新版 ESP-AT 不添加此命令