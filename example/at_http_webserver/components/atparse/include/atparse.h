#ifndef ATPARSE_H
#define ATPARSE_H

#include<stdio.h>
#include<stdint.h>

typedef union{
	struct {
		uint8_t mode;
	}cwmode;
	
	struct {
		char ssid[30];
		char pwd[30];
		char bssid[30];
	}cwjap;

	struct {
		char ssid[30];
		char bssid[30];
     		uint8_t channel;
     		int8_t rssi;
	}queryjap;
	
	struct {
		char ssid[30];
		char pwd[30];
		int8_t chl;
		int8_t ecn;
		int8_t max_conn;
		int8_t ssid_hidden;
	}cwsap;

	struct {
		char* ip;
		char* gateway;
		char* netmask;
	}cipsta;
	
	struct {
		char* ip;
		char* gateway;
		char* netmask;
	}cipap;

	struct {
		char mac[30];
	}cipmac;

	struct {
		uint8_t mode;
	}cipmux;
	
	struct {
		uint8_t mode;
		int8_t port;
		char* ssl;
		int8_t ca_enable;
	}cipserver;

	struct {
		int8_t link_id;
		int32_t length;
	}cipsend;
	struct {
		char* data;
		uint32_t data_len;
	}sendinfo;

	struct {
		char* filename;
	}fsopen;

	struct {
		char* filename;
		uint32_t offset;
		uint32_t length;
	}fsread;

	struct {
		char* ip;
	}ping;

	struct {
		uint8_t enable;
	}autoconn;	
	struct {
		int8_t operate;
		int8_t mode;
	}cwdhcp;
	struct {
		int8_t enable;
		int32_t lease;
		char start_ip[30];
		char end_ip[30];
	}cwdhcps;

	struct {
		int8_t link_id;
		char type[10];
		char remote_ip[30];
		int32_t remote_port;
		int32_t keep_alive;
		int32_t local_port;
		int8_t udp_mode;
	}cipstart;
	struct {
		int8_t link_id;
	}cipclose;
	struct {
		int8_t init_mode;
	}bleinit;

	struct {
		int8_t addr_type;
		char random_addr[30];
	}bleaddr;

	struct {
		char device_name[30];
	}blename;

	struct {
		int8_t scan_type;
		int8_t addr_type;
		int8_t filter_policy;
		int32_t scan_interval;
		int32_t scan_window;
	}blescanparam;

	struct {
		int8_t enable;
		int8_t interval;
	}blescan;

	struct {
		int8_t conn_index;
		char remote_address[30];
	}bleconn;

	struct {
		int8_t conn_id;
	}bledisconn;

	struct {
		int32_t adv_int_min;
		int32_t adv_int_max;
		int8_t adv_type;
		int8_t addr_type;
		int8_t channel;
		int8_t adv_filter_policy;
		int8_t peer_addr_type;
		char peer_address[30];
	}bleadvparam;

	struct {
		char* adv_data;
	}bleadvdata;
}at_cmd_arg;


// Get local IP address and mac, include AP and station.
typedef struct {
     char ap_ip[30];
     char ap_mac[30];
     char sta_ip[30];
     char sta_mac[30];
}cifsr_info;

// Get station ip info
typedef struct {
     char ip[30];
     char gateway[30];
     char netmask[30];
}ip_info;

typedef enum{
    WAIT_CMD = 0,
    AT_CWMODE,   // AT+CWMODE
    AT_CWSAP,    // AT+CWSAP
    AT_CIPMUX,
    AT_CIPSERVER,
    AT_CIPSTA,
    AT_CIPAP,
    AT_CIPSEND,
    AT_SEND_INFO,
    AT_CWJAP,
    AT_CWQAP,
    AT_CWLAP,
    AT_FSOPEN,
    AT_FSREAD,
    AT_SYSRAM,
    AT_CIFSR,
    AT_CWLIF,
    AT_PING,
    AT_AUTOCONN,
    AT_CWDHCP,
    AT_CWDHCPS,
    AT_RST,
    AT_RESTORE,
    AT_CIPSTART,
    AT_CIPCLOSE,
    AT_CIPSTATUS,
    AT_CIPSTAMAC,
    AT_CIPAPMAC,
    AT_QUERTCWJAP,
    AT_QUERTCWSAP,
    AT_QUERTDHCPS,
    AT_QUERTCIPSTA,
    AT_QUERTCIPAP,
    AT_QUERTCIPDNS,
    AT_QUERTDHCP,
    AT_BLEINIT,
    AT_BLEADDR,
    AT_BLENAME,
    AT_BLESCANPARAM,
    AT_BLESCAN,
    AT_BLECONN,
    AT_BLECLOSECONN,
    AT_BLEADVPARAM,
    AT_BLEADVDATA,
    AT_BLEADVSTART,
    AT_BLEADVSTOP,
    AT_QUERTBLEINIT,
    AT_QUERTBLEADDR,
    AT_QUERTBLENAME,
    AT_QUERTBLESCANPARAM,
    AT_QUERTBLECONN,
    AT_QUERTBLEADVPARAM,
}at_command_list;

typedef struct{
	uint8_t rsp_flag;
	void* data;
}at_response;

#define AT_CMD_RETURN_SUCCESS 0
#define AT_CMD_RETURN_FAIL 1
#define AT_ERROR  2

#define at_rsp_error_msg(msg,error_reason) do {\
    (msg)->rsp_flag = AT_ERROR;\
    (msg)->data = error_reason;\
    }while(0)

#define MAX_SCAN_COUNT 20

typedef struct{
	int8_t (*pack)(at_cmd_arg* cmd_arg);
	void (*response)(char* data, size_t len);
}at_cmd_func;

typedef void (*rsp_cb)(char* data, size_t len); 


/**
 * @brief  Add At command
 *
 * @note This is the only API to input AT command and get AT result
 *
 * @param  cmd_list  AT command list.
 *
 * @param  cmd_arg  AT command parameter.
 *
 * @return
 *      - pointer to the struct of AT result, it include whether return ok and the return data.
 */
at_response* at_add_cmd(at_command_list cmd_list, at_cmd_arg* cmd_arg);

/**
 * @brief  Free At command
 *
 * @note It must be called after use "at_add_cmd"
 *
 * @param  rsp  pointer to the struct of AT result.
 *
 */
void at_cmd_free(at_response* rsp);


/**
 * @brief  Init the AT parse frame
 *
 * @note It must be called before use "at_add_cmd" to call AT
 *
 */
void init_parse_frame(void);

#endif
