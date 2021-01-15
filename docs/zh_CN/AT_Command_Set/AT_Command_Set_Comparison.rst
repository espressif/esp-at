***********************************
ESP-AT 不同版本命令集支持对比
***********************************

本文档主要列举了旧版 NONOS-AT 向新版 ESP-AT 迁移时，需要注意的差异。

- `NONOS-AT`：不推荐使用的 ESP8266 AT 版本，早期用于 ESP8266 系列芯片，基于 ESP8266_NONOS_SDK 开发，已停止更新。
- `ESP-AT`：推荐使用的版本，支持多个芯片平台，具体请参阅 :doc:`../Get_Started/What_is_ESP-AT` 和 :doc:`../AT_Binary_Lists/index`。

.. list-table:: ESP-AT 不同版本命令集支持对比表
   :header-rows: 1

   * - 命令
     - 简介
     - NONOS-AT
     - ESP-AT
     - 备注
   * - AT
     - 测试 AT 启动
     - 是
     - 是
     - /
   * - AT+RST
     - 重启模块
     - 是
     - 是
     - /
   * - AT+GMR
     - 查询版本信息
     - 是
     - 是
     - /
   * - AT+GSLP
     - 进入 deep-sleep 模式
     - 是
     - 是
     - /
   * - ATE
     - 开关回显功能
     - 是
     - 是
     - /
   * - AT+RESTORE
     - 恢复出厂设置
     - 是
     - 是
     - /
   * - AT+UART_CUR
     - 配置 UART，不保存到 Flash
     - 是
     - 是
     - /
   * - AT+UART_DEF
     - 配置 UART，保存到 Flash
     - 是
     - 是
     - /
   * - AT+SLEEP
     - 设置 sleep 模式
     - 是，1：light sleep<br>，2：modem sleep
     -
        * ESP8266：是，1：modem sleep by DTIM，2：light sleep，3：modem sleep by listen interval
        * ESP32：是，1：modem sleep by DTIM，2：light sleep，3：modem sleep by listen interval
        * ESP32-S2：无
     - /
   * - AT+WAKEUPGPIO
     - 配置 GPIO 唤醒 light sleep
     - 是
     - 无，与之相似功能命令参考：AT+SLEEPWKCFG
     - ESP-AT 不添加此命令
   * - AT+RFPOWER
     - 设置 RF TX Power
     - 是，范围：[0,82]，单位：0.25 dBm
     -
       * ESP8266：是，范围：[40,82]，单位：0.25 dBm
       * ESP32：是，范围：[40,78]，单位：0.25 dBm，且支持 Bluetooth LE
       * ESP32-S2：是，范围：[40,78]，单位：0.25 dBm
     - /
   * - AT+RFVDD
     - 根据 VDD33 设置 RF TX Power
     - 是
     - 无
     - /
   * - AT+SYSRAM
     - 查询系统当前剩余内存
     - 是
     - 是，新增最小峰值内存参数
     - /
   * - AT+SYSADC
     - 查询 ADC 值
     - 是
     - 无
     - /
   * - AT+SYSIOSETCFG
     - 设置 IO 工作模式
     - 是
     - 无
     - /
   * - AT+SYSIOGETCFG
     - 查询 IO 工作模式
     - 是
     - 无
     - /
   * - AT+SYSGPIODIR
     - 设置 GPIO 工作为输入或输出
     - 是
     - 无
     - /
   * - AT+SYSGPIOWRITE
     - 设置 GPIO 的输出电平
     - 是
     - 无
     - /
   * - AT+SYSGPIOREAD
     - 读取 GPIO 的电平状态
     - 是
     - 无
     - /
   * - AT+SYSMSG
     - 设置系统消息
     - 无
     - 是
     - /
   * - AT+SYSMSG_CUR
     - 设置当前系统消息，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+SYSMSG
     - ESP-AT 不添加此命令
   * - AT+SYSMSG_DEF
     - 设置默认系统消息，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+SYSMSG
     - ESP-AT 不添加此命令
   * - AT+CWMODE
     - 设置 Wi-Fi 模式
     - 无
     - 是，新增切换模式自动连接
     - /
   * - AT+CWMODE_CUR
     - 设置当前 Wi-Fi 模式，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWMODE
     - ESP-AT 不添加此命令
   * - AT+CWMODE_DEF
     - 设置当前 Wi-Fi 模式，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWMODE
     - ESP-AT 不添加此命令
   * - AT+CWJAP
     - 连接 AP
     - 是
     - 是，新增更多功能
     - /
   * - AT+CWJAP_CUR
     - 连接 AP，参数不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWJAP
     - ESP-AT 不添加此命令
   * - AT+CWJAP_DEF
     - 连接 AP，参数保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWJAP
     - ESP-AT 不添加此命令
   * - AT+CWLAPOPT
     - 设置 CWLAP 指令的属性
     - 是
     - 是，新增更多功能
     - /
   * - AT+CWLAP
     - 扫描当前可⽤ AP
     - 是
     - 是，回复不同
     - /
   * - AT+CWQAP
     - 断开与 AP 连接
     - 是
     - 是
     - /
   * - AT+CWSAP
     - 配置 SoftAP 参数
     - 无
     - 是
     - /
   * - AT+CWSAP_CUR
     - 配置 SoftAP 参数，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWSAP
     - ESP-AT 不添加此命令
   * - AT+CWSAP_DEF
     - 配置 SoftAP 参数，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWSAP
     - ESP-AT 不添加此命令
   * - AT+CWLIF
     - 查询连接到 SoftAP 的 Station 信息
     - 是
     - 是
     - /
   * - AT+CWDHCP
     - 设置 DHCP
     - 无
     - 是
     - /
   * - AT+CWDHCP_CUR
     - 设置 DHCP，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWDHCP
     - ESP-AT 不添加此命令
   * - AT+CWDHCP_DEF
     - 设置 DHCP，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWDHCP
     - ESP-AT 不添加此命令
   * - AT+CWDHCPS
     - 设置 SoftAP DHCP 分配的 IP 范围
     - 无
     - 是
     - /
   * - AT+CWDHCPS_CUR
     - 设置 SoftAP DHCP 分配的 IP 范围，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWDHCPS
     - ESP-AT 不添加此命令
   * - AT+CWDHCPS_DEF
     - 设置 SoftAP DHCP 分配的 IP 范围，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWDHCPS
     - ESP-AT 不添加此命令
   * - AT+CWAUTOCONN
     - 上电是否自动连接 AP
     - 是
     - 是
     - /
   * - AT+CIPSTAMAC
     - 设置 Station 接⼝的 MAC 地址
     - 无
     - 是
     - /
   * - AT+CIPSTAMAC_CUR
     - 设置 Station 接口 MAC 地址，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPSTAMAC
     - ESP-AT 不添加此命令
   * - AT+CIPSTAMAC_DEF
     - 设置 Station 接口 MAC 地址，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPSTAMAC
     - ESP-AT 不添加此命令
   * - AT+CIPAPMAC
     - 设置 SoftAP 的 MAC 地址
     - 无
     - 是
     - /
   * - AT+CIPAPMAC_CUR
     - 设置 SoftAP 的 MAC 地址，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPAPMAC
     - ESP-AT 不添加此命令
   * - AT+CIPAPMAC_DEF
     - 设置 SoftAP 的 MAC 地址，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPAPMAC
     - ESP-AT 不添加此命令
   * - AT+CIPSTA
     - 设置 Station 的 IP 地址
     - 无
     - 是
     - /
   * - AT+CIPSTA_CUR
     - 设置 Station 的 IP 地址，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPSTA
     - ESP-AT 不添加此命令
   * - AT+CIPSTA_DEF
     - 设置 Station 的 IP 地址，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPSTA
     - ESP-AT 不添加此命令
   * - AT+CIPAP
     - 设置 SoftAP 的 IP 地址
     - 无
     - 是
     - /
   * - AT+CIPAP_CUR
     - 设置 SoftAP 的 IP 地址，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPAP
     - ESP-AT 不添加此命令
   * - AT+CIPAP_DEF
     - 设置 SoftAP 的 IP 地址，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPAP
     - ESP-AT 不添加此命令
   * - AT+CWSTARTSMART
     - 开启 SmartConfig
     - 是
     - 是
     - /
   * - AT+CWSTOPSMART
     - 停止 SmartConfig
     - 是
     - 是
     - /
   * - AT+CWSTARTDISCOVER
     - 开启可被局域⽹网内微信探测模式
     - 是
     - 无
     - /
   * - AT+CWSTOPDISCOVER
     - 关闭可被局域⽹网内微信探测模式
     - 是
     - 无
     - /
   * - AT+WPS
     - 设置 WPS 功能
     - 是
     - 是
     - /
   * - AT+MDNS
     - 设置 MDNS 功能
     - 是
     - 是
     - /
   * - AT+CWJEAP
     - 连接企业级加密路由器
     - 无
     -
       * ESP8266：无
       * ESP32：是
       * ESP32-S2：无
     - /
   * - AT+CWJEAP_CUR
     - 连接企业级加密路由器，不保存到 Flash
     - 是
     - 无
     - /
   * - AT+CWJEAP_DEF
     - 连接企业级加密路由器，保存到 Flash
     - 是
     - 无
     - /
   * - AT+CWHOSTNAME
     - 设置 Station 的主机名称
     - 是
     - 是
     - /
   * - AT+CWCOUNTRY
     - 设置 Wi-Fi 国家码
     - 无
     - 是
     - /
   * - AT+CWCOUNTRY_CUR
     - 设置 Wi-Fi 国家码，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWCOUNTRY
     - ESP-AT 不添加此命令
   * - AT+CWCOUNTRY_DEF
     - 设置 Wi-Fi 国家码，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CWCOUNTRY
     - ESP-AT 不添加此命令
   * - AT+CIPSTATUS
     - 查询⽹网络连接信息
     - 是
     - 是
     - /
   * - AT+CIPDOMAIN
     - 域名解析功能
     - 是
     - 是
     - /
   * - AT+CIPSTART
     - 建立 TCP/UDP/SSL 连接
     - 是
     - 是
     - /
   * - AT+CIPSSLSIZE
     - 设置 SSL Buffer size
     - 是
     - 无
     - /
   * - AT+CIPSSLCCONF
     - 配置 SSL 连接属性
     - 是
     - 是，参数不同
     - /
   * - AT+CIPSEND
     - 发送数据
     - 是
     - 是
     - /
   * - AT+CIPSENDEX
     - 发送长度为 length 数据，或在长度内 \0 结尾的数据
     - 是
     - 是
     - /
   * - AT+CIPSENDBUF
     - 数据写入 TCP 发包缓存
     - 是
     - 无
     - ESP-AT 不添加此命令
   * - AT+CIPBUFRESET
     - 重新计数
     - 是
     - 无
     - ESP-AT 不添加此命令
   * - AT+CIPBUFSTATUS
     - 查询 TCP 发包缓存的状态
     - 是
     - 无
     - ESP-AT 不添加此命令
   * - AT+CIPCHECKSEQ
     - 查询写入 TCP 发包缓存的某包是否发送成功
     - 是
     - 无
     - ESP-AT 不添加此命令
   * - AT+CIPCLOSEMODE
     - 设置 TCP 连接的断开方式
     - 是
     - 无
     - ESP-AT 不添加此命令
   * - AT+CIPCLOSE
     - 关闭 TCP/UDP/SSL 传输
     - 是
     - 是
     - /
   * - AT+CIFSR
     - 查询本地 IP 地址
     - 是
     - 是
     - /
   * - AT+CIPMUX
     - 设置多连接
     - 是
     - 是
     - /
   * - AT+CIPSERVER
     - 建立 TCP 服务器
     - 是，不支持 SSL server
     -
       * ESP8266：是，不支持 SSL server
       * ESP32：是，支持 SSL server
       * ESP32-S2：是，支持 SSL server
     - /
   * - AT+CIPSERVERMAXCONN
     - 设置服务器允许建立的最⼤连接数
     - 是
     - 是
     - /
   * - AT+CIPMODE
     - 设置传输模式
     - 是
     - 是
     - /
   * - AT+SAVETRANSLINK
     - 保存透传到 Flash
     - 是
     - 是
     - /
   * - AT+CIPSTO
     - 设置 TCP 服务器器超时时间
     - 是
     - 是
     - /
   * - AT+PING
     - Ping 功能
     - 是
     - 是
     - /
   * - AT+CIUPDATE
     - 通过 Wi-Fi 升级固件
     - 是
     - 是，支持更多参数
     - /
   * - AT+CIPDINFO
     - 接收网络数据时是否提示对端 IP 和端⼝
     - 是
     - 是
     - /
   * - AT+CIPRECVMODE
     - 设置 TCP 连接的数据接收方式
     - 是
     - 是
     - /
   * - AT+CIPRECVDATA
     - 被动接收模式时，读取缓存的 TCP 数据
     - 是
     - 是，回复有差异
     - /
   * - AT+CIPRECVLEN
     - 被动接收模式时，查询缓存 TCP 数据的长度
     - 是
     - 是
     - /
   * - AT+CIPSNTPCFG
     - 设置时域和 SNTP 服务器
     - 是
     - 是，支持更多功能
     - /
   * - AT+CIPSNTPTIME
     - 查询 SNTP 时间
     - 是
     - 是
     - /
   * - AT+CIPDNS
     - ⾃定义 DNS 服务器
     - 无
     - 是
     - /
   * - AT+CIPDNS_CUR
     - 自定义 DNS 服务器，不保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPDNS
     - ESP-AT 不添加此命令
   * - AT+CIPDNS_DEF
     - 自定义 DNS 服务器，保存到 Flash
     - 是
     - 无，与之相似功能命令参考：AT+CIPDNS
     - ESP-AT 不添加此命令
   * - AT+SYSFLASH
     - 读写 Flash 用户分区
     - 是
     - 是
     - /
   * - AT+FS
     - ⽂件系统操作
     - 无
     -
       * ESP8266：无
       * ESP32：是
       * ESP32-S2：是
     - /
   * - AT+BT..
     - BT 相关命令
     - 无
     -
       * ESP8266：无
       * ESP32：是
       * ESP32-S2：无
     - /
   * - AT+BLE..
     - Bluetooth LE 相关命令
     - 无
     -
       * ESP8266：无
       * ESP32：是
       * ESP32-S2：无
     - /
   * - AT+ETH..
     - ETH 相关命令
     - 无
     -
       * ESP8266：无
       * ESP32：是
       * ESP32-S2：无
     - /
