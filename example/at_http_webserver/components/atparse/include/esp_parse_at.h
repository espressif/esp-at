/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __ESPPARSEAT_H__
#define __ESPPARSEAT_H__

#include "esp_err.h"
#include "atparse.h"

typedef enum {
    QUERY,   // Query the current value of parameters
    SET,     // Set the value of user-defined parameters, and runs it.
} esp_at_method;

typedef enum {
    CONNECT_AP = 2,        // Station is connected to an AP and its IP is obtained.
    CREAT_TRAN = 3,        // Station has created a TCP or UDP transmission.
    DISCONN_TRAN = 4,      // The TCP or UDP transmission of ESP32 Station is disconnected.
    DISCONN_AP = 5,        // ESP32 Station does NOT connect an AP.
} sta_interface_status;

typedef enum {
    STATION,
    SOFTAP,
} esp_wifi_mode;

typedef enum {
    NONE = 0,            // Disable station DHCP and softap DHCP.
    CLIENT,              // Enable station DHCP.
    SERVER,              // Enable softap DHCP.
    BOTH,                // Enable station DHCP and softap DHCP.
} esp_dhcp_status;

typedef union {
    struct {
        uint8_t mode;                // WiFi mode(0-3), 0:NULL mode, 1:station mode, 2:softap mode, 3:softap+station mode
    } cwmode;

    struct {
        char ssid[33];               // the SSID of the target AP
        char pwd[64];                // password
        char bssid[20];              // (optional parameter) the target AP's MAC address, NULL if not use.
    } cwjap;

    struct {
        char ssid[33];               // show the SSID of the AP
        char bssid[20];              // show the AP's MAC address
        uint8_t channel;
        int8_t rssi;                 // signal strength
    } queryjap;

    struct {
        char ssid[33];               // ssid of AP
        char pwd[64];                // password
        int8_t chl;                  // channel ID
        int8_t ecn;                  // encryption method(0-4), 0:OPEN, 1:WEP, 2:WPA_PSK, 3:WPA2_PSK, 4:WPA_WPA2_PSK
        int8_t max_conn;             // (optional parameter), maximum number of station to which ESP32 softAP can be connect. Range(1,10)
        int8_t ssid_hidden;          // (optional parameter), ssid  hidden(0 or 1), 0 - ssid is broadcast(the default setting), 1 - ssid is not broadcast
    } cwsap;

    struct {
        char* ip;
        char* gateway;
        char* netmask;
    } ipinfo;     // cipsta or cipap

    struct {
        uint8_t enable;              // (0 or 1) : 1 - enable auto connect , 0 - disable auto connect
    } autoconn;

    struct {
        int8_t enable;               // 0 or 1, 0 - disable the settings and use the default IP range, 1 - enable setting the IP range, and the parameters below have to be set
        int32_t lease;               // lease time, unit:minute, range [1,2880]
        char start_ip[20];           // start IP of the IP range that can be obtained from ESP32 softAP DHCP server
        char end_ip[20];             // end IP of the IP range
    } cwdhcps;

    struct {
        char mac[20];
    } cipmac;

    struct {
        int8_t link_id;              // ID of network connection (0~max_conn)
        char type[10];               // connection type: "TCP", "UDP" or "SSL"
        char remote_ip[20];          // remote IP address
        int32_t remote_port;         // remote port number
        int32_t keep_alive;          // detection time interval when TCP is kept alive, default is disabled. If establish UDP connect , it is omit,(-1 is a good choice)
        int32_t local_port;          // UDP port. It only used by UDP connect, and it is optional parameter
        int8_t udp_mode;             // UDP mode(0-2). 0 - destination peer entity of UDP will not change, 1 - change once, 2 - allow to change.
        // It only used by UDP connect, and it also is optional parameter
    } cipstart;

    struct {
        char filename[256];
    } fsopen;

    struct {
        char filename[256];
        uint32_t offset;
        uint32_t length;
    } fsread;

    struct {
        int8_t init_mode;            // init role(0 or 2), 0 - not init, 1 - client role, 2 - server role
    } bleinit;

    struct {
        int8_t addr_type;            // address type(0 or 1), 0 - public address, 1 - random address
        char random_addr[20];
    } bleaddr;

    struct {
        char device_name[30];        // BLE device name
    } blename;

    struct {
        int8_t scan_type;            // BLE scan type(0 or 1), 0 - passive scan, 1 - active scan
        int8_t addr_type;            // BLE own address type(0 ~ 3), 0 - public address, 1 - random address, 2 - RPA public address, 3 - PRA random address
        int8_t filter_policy;        // filter policy(0 ~ 3), 0 - ALLOW_ALL, 1 - ALLOW_ONLY_WLIST, 2 - ALLOW_UND_RPA_DIR, 3 - ALLOW_WLIST_PRA_DIR
        int32_t scan_interval;       // scan interval
        int32_t scan_window;         // scan window
    } blescanparam;

    struct {
        int8_t enable;
        int8_t interval;
    } blescan;

    struct {
        int8_t conn_index;
        char remote_address[20];
    } bleconn;

    struct {
        int32_t adv_int_min;
        int32_t adv_int_max;
        int8_t adv_type;
        int8_t addr_type;
        int8_t channel;
        int8_t adv_filter_policy;
        int8_t peer_addr_type;
        char peer_address[20];
    } bleadvparam;

} esp_at_arg;

typedef struct {
    char ip[20];
    char mac[20];
    char gateway[20];
    char netmask[20];
} esp_at_ip_info;

typedef struct scan_ap_struct {
    uint8_t ecn;                       // encryption menthod (0-4), 0:OPEN, 1:WEP, 2:WPA_PSK, 3:WPA2_PSK, 4:WPA_WPA2_PSK
    char ssid[33];                     // string parameter, SSID of the AP.
    int8_t rssi;                       // signal strength
    char mac[20];                      // string parameter, MAC address of the AP.
    uint8_t channel;                   // channel of the AP.
    struct scan_ap_struct* next;
} scan_ap_link;

typedef struct at_ble_scan_info {
    char addr[20];
    int rssi;
    char adv_data[63];
    char scan_rsp_data[63];    // 31*2
    struct at_ble_scan_info* next;
} ble_scan_info;

typedef struct connect_status {
    uint8_t link_id;                    // ID of the connection(0~max).
    char type[10];                      // string parameter, "TCP" or "UDP".
    char remote_ip[20];                 // string parameter indicating the remote IP address.
    uint32_t remote_port;               // the remote port number.
    uint32_t local_port;                // ESP32 local port number.
    uint8_t tetype;                     // 0 or 1, 0 means ESP32 runs as a client, 1 means ESP32 runs as a server.
    struct connect_status* next;
} conn_status_link;

typedef struct {
    sta_interface_status interface_status;     // status of the ESP32 Station interface.
    conn_status_link* status_link;
} conn_stat;

typedef struct connected_dev {
    char ip[20];
    char mac[20];
    struct connected_dev* next;
} conn_dev_link;

typedef struct ble_connected_dev {
    uint8_t conn_index;               // index of BLE connection
    char addr[20];                    // remote BLE address
    struct ble_connected_dev* next;
} ble_conn_dev;

/**
 * @brief BLE scan callback function
 *
 * @param dev_head pointer to BLE scan info linked list head
 *
 * @note  The link list need be freed by yourself.
 */
typedef void (* ble_scan_cb)(ble_scan_info* dev_head);

/**
 * @brief WiFi scan callback function
 *
 * @param dev_head pointer to WiFi scan linked list head
 *
 * @note  The link list need be freed by yourself.
 */
typedef void (* wifi_scan_cb)(scan_ap_link* dev_head);

/**
 * @brief    Join Access point
 *
 * @note     This API can join ap or get joined ap info.
 *
 * @param    method : join or get current ap info (set/query)
 *
 * @param    arg    : cwjap / queryjap.
 *              When use set method, arg is cwjap. When use query method, arg is queryjap.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_join_ap(esp_at_method method, esp_at_arg* arg);

/**
  * @brief     Set auto connect
  *            The default value is true
  *
  * @param     method : Get the auto connect flag (QUERY) / set auto connect (SET)
  *
  * @param     arg    : autoconn
  *
  * @return
  *    - ESP_OK: succeed
 *     - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *     - ESP_ERR_INVALID_STATE if AT core execute error
  */
esp_err_t esp_at_auto_connect(esp_at_method method, esp_at_arg* arg);

/**
 * @brief    Get the IP information which connected to ESP32 softAP
 *
 * @note     This API cannot get a station IP.
 *
 * @return
 *      - NULL if fail or no connection
 *      - connected IP infor link
 * @note IP infor link need to be freed by yourself
 */
conn_dev_link* esp_at_connected_device(void);

/**
 * @brief    enable/disable DHCP
 *
 * @note     this configuration changes will be stored in the NVS area.
 *
 * @param    method : set DHCP or get current DHCP info (set/query)
 *
 * @param    esp_dhcp_status : DHCP status
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_dhcp_status(esp_at_method method, esp_dhcp_status* dhcp_status);

/**
 * @brief    Set the IP address allocated by softAP DHCP
 *
 * @note     this configuration changes will be stored in the NVS area.
 *
 * @param    method : set IP address allocate or get current IP address allocate (set/query)
 *
 * @param    arg    : cwdhcps
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_set_dhcps(esp_at_method method, esp_at_arg* arg);

/**
 * @brief    Disconnect from AP
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_close_ap(void);

/**
 * @brief    Config Access point info
 *
 * @note     this configuration changes will be stored in the NVS area.
 *
 * @param    method : set or get configuration parameters of the ESP32 softAP (set/query)
 *
 * @param    arg  : cwsap.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_set_ap(esp_at_method method, esp_at_arg* arg);

/**
 * @brief    Get IP info
 *
 * @param    mode : WiFi mode, station or softAP.
 *
 * @param    ip_info : ipinfo
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_get_ip(esp_wifi_mode mode, esp_at_ip_info* ip_info);

/**
 * @brief    Set IP info
 *
 * @param    mode : WiFi mode, station or softAP.
 *
 * @param    arg  : ipinfo
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_set_ip(esp_wifi_mode mode, esp_at_arg* arg);

/**
 * @brief    Set MAC info
 *
 * @param    mode : WiFi mode, station or softAP.
 *
 * @param    arg  : cipmac
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_set_mac(esp_wifi_mode mode, esp_at_arg* arg);

/**
 * @brief    Get AP list in last scan
 *
 * @param    cb : wifi_scan_cb hold the found APs info
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_wifi_scan(wifi_scan_cb cb);

/**
 * @brief    Get the remaining space of RAM
 *
 * @return
 *      - -1 if AT core execute error
 *      - remain RAM size
 */
int32_t esp_at_getramsize(void);

/**
 * @brief    Send ping packets
 *
 * @param    addr  : host IP or domain name
 *
 * @return
 *      - ping time
 *      - -1 if AT core execute error
 *      - -2 if the configuration values are not correct
 */
int32_t esp_at_ping(char* addr);

/**
 * @brief    restart system
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_restart(void);

/**
 * @brief    restore system
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_restore(void);

/**
 * @brief    Establish TCP/UDP/SSL connection
 *
 * @param    arg  : cipstart
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_establish_connect(esp_at_arg* arg);

/**
 * @brief    Close TCP/UDP/SSL connection
 *
 * @param    link_id  : ID number of connections to be closed, when ID=5, all connections will be closed
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_close_connect(int8_t link_id);

/**
 * @brief  Gets the connect status
 *
 * @return
 *      - a struct include a IP link list, this struct and link list need be freed by yourself.
 *      - NULL if error
 */
conn_stat* esp_at_get_connect_status(void);

/**
 * @brief  Open a FATFS file
 *
 * @return
 *      - len if success
 *      - -1 if error
 */
int32_t esp_at_fatfs_open(char* filename);

/**
 * @brief  Read FATFS file
 *
 * @return
 *      - Point to address where stores read contents if success
 *      - NULL if error
 */
char* esp_at_fatfs_read(char* filename, uint32_t offset, uint32_t length);

/**
 * @brief    BLE initialization
 *
 * @note     If ont want to change the BLE role, restart first.
 *
 * @param    method : init BLE role or get BLE init status (set/query)
 *
 * @param    arg  : bleinit.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_init(esp_at_method method, esp_at_arg* arg);

/**
 * @brief    Set BLE device's address
 *
 * @note     Now only support set BLE random address and get BLE public address
 *
 * @param    method : set BLE random address or get BLE public address (set/query)
 *
 * @param    arg  : bleaddr.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_address(esp_at_method method, esp_at_arg* arg);

/**
 * @brief    Set BLE device's name
 *
 * @note     The default BLE device name is "BLE_AT"
 *
 * @param    method : set or get BLE public name (set/query)
 *
 * @param    arg  : blename.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_name(esp_at_method method, esp_at_arg* arg);

/**
 * @brief    Set BLE parameters of advertising
 *
 * @note     <adv_filter_policy>,<peer_addr_type>,<peer_addr> these three parameters should be set
together, or be omitted together
 *
 * @param    method : set or get BLE advertise parameters (set/query)
 *
 * @param    arg  : bleadvparam.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_adv_param(esp_at_method method, esp_at_arg* arg);

/**
 * @brief    Set parameters of BLE scanning
 *
 * @note     scan_window cannot be larger than scan_interval
 *
 * @param    method : set or get the parameter of BLE scanning (set/query)
 *
 * @param    arg  : blescanparam.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_scan_param(esp_at_method method, esp_at_arg* arg);

/**
 * @brief    Set BLE advertising data
 *
 * @note     The maximum length of the advertising data is 31 bytes
 *
 * @param    adv_data : advertising data; this is a HEX string
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the configuration values are not correct
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_set_adv_data(char* adv_data);

/**
 * @brief    Start advertising
 *
 * @note     If advertising parameters are NOT set by command AT+BLEADVPARAM=<adv_parameter>, the default parameters will be used.
 *
 * @note     If advertising data is NOT set by command AT+BLEADVDATA=<adv_data>, the all zeros data will be sent.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_adv_start();

/**
 * @brief    Stop advertising
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_adv_stop();

/**
 * @brief    Enable BLE scanning
 *
 * @note     Now only support scan 1s
 *
 * @param    arg  : blescanparam.
 *
 * @param    cb : ble_scan_cb hold the found BLEs info
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_scan(esp_at_arg* arg, ble_scan_cb cb);

/**
 * @brief    Connect to BLE server
 *
 * @note     It just try to connect ble server, but not care whether it success,
 *           using "esp_at_get_ble_connect" to obtain whether the connection is successful
 *
 * @param    arg : bleconn.
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_connect(esp_at_arg* arg);

/**
 * @brief    Get BLE connection info
 *
 * @note     It just try to connect ble server, but not care whether it success,
 *
 * @return
 *      - ble device link list, it need be freed by yourself.
 *      - NULL if error or no connect device
 */
ble_conn_dev* esp_at_get_ble_connect(void);

/**
 * @brief    Close BLE connection
 *
 * @param    conn_id : index of BLE connection
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_ble_close_connect(uint8_t conn_id);

/**
 * @brief   Init the AT parse frame
 *
 * @note    It must be called fistly
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if AT core execute error
 */
esp_err_t esp_at_init_parse(void);

#endif
