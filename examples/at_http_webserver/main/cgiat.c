#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

#include "esp_log.h"
#include "nvs.h"
#include "cgiat.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
static const char* TAG = "cgiAT";

static const char* nvm_login_pwd  = "NVM_CGIAT";

cJSON* root = NULL;


// change BLE init mode
int8_t set_ble_mode(int8_t set_mode)
{
    esp_err_t ret;
    int8_t cur_init_mode = -1;
    esp_at_arg at_arg;

    if (set_mode > 2 || set_mode < 0) {
        ESP_LOGE(TAG, "Error mode:%d", set_mode);
        return -1;
    }

    ret = esp_at_ble_init(QUERY, &at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        return -1;
    }

    cur_init_mode = at_arg.bleinit.init_mode;
    ESP_LOGI(TAG, "Current ble init mode:%d", cur_init_mode);

    if (cur_init_mode == 0) {
        at_arg.bleinit.init_mode = set_mode;
        ESP_LOGI(TAG, "Current ble init mode:%d, will set to mode %d.", cur_init_mode, at_arg.bleinit.init_mode);
        ret = esp_at_ble_init(SET, &at_arg);

        if (ret == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "Error parameter\n");
            return -1;
        }
    } else if (cur_init_mode == 1) {
        if (set_mode == 2) {
            ESP_LOGI(TAG, "I will close BLE client mode firstly");
            at_arg.bleinit.init_mode = 0;
            ret = esp_at_ble_init(SET, &at_arg);

            if (ret == ESP_ERR_INVALID_STATE) {
                ESP_LOGE(TAG, "Error parameter\n");
                return -1;
            }

            vTaskDelay(100 / portTICK_RATE_MS);   // wait some time
            ESP_LOGI(TAG, "Set to server mode");
            at_arg.bleinit.init_mode = set_mode;
            ret = esp_at_ble_init(SET, &at_arg);

            if (ret == ESP_ERR_INVALID_STATE) {
                ESP_LOGE(TAG, "Error parameter\n");
                return -1;
            }
        }
    } else if (cur_init_mode == 2) {
        if (set_mode == 1) {
            ESP_LOGI(TAG, "I will close BLE server mode firstly");
            at_arg.bleinit.init_mode = 0;
            ret = esp_at_ble_init(SET, &at_arg);

            if (ret == ESP_ERR_INVALID_STATE) {
                ESP_LOGE(TAG, "Error parameter\n");
                return -1;
            }

            vTaskDelay(100 / portTICK_RATE_MS);   // wait some time
            ESP_LOGI(TAG, "Set to server mode");
            at_arg.bleinit.init_mode = set_mode;
            ret = esp_at_ble_init(SET, &at_arg);

            if (ret == ESP_ERR_INVALID_STATE) {
                ESP_LOGE(TAG, "Error parameter\n");
                return -1;
            }
        }
    }

    return 0;
}

void reponseError(HttpdConnData* connData, uint32_t error_code)
{
    char* printed_json = NULL;
    cJSON* error_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(error_root, "state", 1);
    cJSON_AddNumberToObject(error_root, "error_code", error_code);
    httpdStartResponse(connData, 200);
    //We are going to send some json data.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(error_root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(error_root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }
}

void reponseOk(HttpdConnData* connData)
{
    char* printed_json = NULL;
    cJSON* correct_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(correct_root, "state", 0);
    httpdStartResponse(connData, 200);
    //We are going to send some json data.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(correct_root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(correct_root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }
}

/*
* Request: loginpwd=123456
* Response:{"state":0(correct pwd) or 1 (error pwd)}
*/
int cgiloginpwd(HttpdConnData* connData)
{
    int len = 0;
    char loginpwd[100];
    int login_pwd;
    uint32_t nvs_loginpwd;
    nvs_handle handle;

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);  //http error code 'unacceptable'
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "loginpwd", loginpwd, sizeof(loginpwd));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400); //http request error
        return HTTPD_CGI_DONE;
    }

    login_pwd = atoi(loginpwd);

    if (nvs_open(nvm_login_pwd, NVS_READWRITE, &handle) != ESP_OK) {
        reponseError(connData, 500); // http server inner error
        return HTTPD_CGI_DONE;
    }

    if (nvs_get_u32(handle, "loginpwd", &nvs_loginpwd) != ESP_OK) {
        ESP_LOGI(TAG, "No login password in nvs,use the default login password:123456");
        nvs_loginpwd = 123456;

        if (nvs_set_u32(handle, "loginpwd", nvs_loginpwd) != ESP_OK) {
            nvs_close(handle);
            reponseError(connData, 500); // http server inner error
            return HTTPD_CGI_DONE;
        }

        nvs_commit(handle);
    }


    if (nvs_loginpwd != login_pwd) {
        ESP_LOGI(TAG, "Login password error");
        reponseError(connData, 600); // password error
    } else {
        ESP_LOGI(TAG, "Login password correct");
        reponseOk(connData);
    }

    nvs_close(handle);
    return HTTPD_CGI_DONE;

}

/*
*  Request: oldpwd=123456&newpwd=12345678
*
*  Response: {"state":0(success) or 1 (fail)}
*
*/

int cgiChangeLoginPwd(HttpdConnData* connData)
{
    int len;
    char oldpwd[100];
    char newpwd[100];
    uint32_t login_pwd;
    uint32_t new_pwd;
    uint32_t nvs_loginpwd;
    nvs_handle handle;

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406); //http error code 'unacceptable'
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "oldpwd", oldpwd, sizeof(oldpwd));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400); //http request error
        return HTTPD_CGI_DONE;
    }

    login_pwd = atoi(oldpwd);

    len = httpdFindArg(connData->post->buff, "newpwd", newpwd, sizeof(newpwd));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    new_pwd = atoi(newpwd);

    if (nvs_open(nvm_login_pwd, NVS_READWRITE, &handle) != ESP_OK) {
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    if (nvs_get_u32(handle, "loginpwd", &nvs_loginpwd) != ESP_OK) {
        ESP_LOGI(TAG, "Cannot get nvs login password");
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        nvs_close(handle);
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    if (nvs_loginpwd != login_pwd) {
        ESP_LOGI(TAG, "Old login password error");
        reponseError(connData, 600);
    } else {
        if (nvs_set_u32(handle, "loginpwd", new_pwd) != ESP_OK) {
            nvs_close(handle);
            reponseError(connData, 500);
            return HTTPD_CGI_DONE;
        }

        nvs_commit(handle);
        reponseOk(connData);
    }

    nvs_close(handle);
    return HTTPD_CGI_DONE;

}

int cgiRouteInfo(HttpdConnData* connData)
{
    esp_err_t ret;
    char* printed_json = NULL;
    esp_at_ip_info ip_info;
    memset(&ip_info, '\0', sizeof(ip_info));

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ret = esp_at_get_ip(SOFTAP, &ip_info);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Get softap info");
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);
    cJSON_AddStringToObject(root, "apip", ip_info.ip);
    cJSON_AddStringToObject(root, "apmac", ip_info.mac);
    cJSON_AddStringToObject(root, "apgateway", ip_info.gateway);
    cJSON_AddStringToObject(root, "apnetmask", ip_info.netmask);
    ESP_LOGI(TAG, "Softap info:ip:%s,mac:%s,gateway:%s,netmask:%s", ip_info.ip, ip_info.mac, ip_info.gateway, ip_info.netmask);
    memset(&ip_info, '\0', sizeof(ip_info));
    ret = esp_at_get_ip(STATION, &ip_info);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        cJSON_Delete(root);
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Get station info");
    cJSON_AddStringToObject(root, "staip", ip_info.ip);
    cJSON_AddStringToObject(root, "stamac", ip_info.mac);
    cJSON_AddStringToObject(root, "stagateway", ip_info.gateway);
    cJSON_AddStringToObject(root, "stanetmask", ip_info.netmask);
    ESP_LOGI(TAG, "Station info:ip:%s,mac:%s,gateway:%s,netmask:%s", ip_info.ip, ip_info.mac, ip_info.gateway, ip_info.netmask);

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}


int cgiShowApInfo(HttpdConnData* connData)
{
    esp_at_arg at_arg;
    char* printed_json = NULL;

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    esp_err_t ret = esp_at_join_ap(QUERY, &at_arg); // Get AP info

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error pwd or ssid\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);
    cJSON_AddStringToObject(root, "ssid", at_arg.queryjap.ssid);
    cJSON_AddStringToObject(root, "bssid", at_arg.queryjap.bssid);
    cJSON_AddNumberToObject(root, "channel", at_arg.queryjap.channel);
    cJSON_AddNumberToObject(root, "rssi", at_arg.queryjap.rssi);


    ESP_LOGD(TAG, "Ip info data:%s\n", cJSON_Print(root));

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}

int cgirst(HttpdConnData* connData)
{
    esp_err_t ret = esp_at_restart();

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    return HTTPD_CGI_DONE;
}

int cgirestore(HttpdConnData* connData)
{
    esp_err_t ret = esp_at_restore();

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error pwd or ssid\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    return HTTPD_CGI_DONE;
}

/*
*
*  Request: NULL
*  Response: NULL
*
*/

int cgiCloseAp(HttpdConnData* connData)
{
    if (esp_at_close_ap() != ESP_OK) {
        ESP_LOGE(TAG, "command execution error");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);// when close ap, maybe will close the connection, so client cannot receive response.
    return HTTPD_CGI_DONE;
}

int getConnDev(HttpdConnData* connData)
{
    char* printed_json = NULL;
    conn_dev_link* connected_dev = NULL;

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    connected_dev = esp_at_connected_device();

    if (connected_dev == NULL) {
        httpdStartResponse(connData, 200);
        httpdEndHeaders(connData);
        return HTTPD_CGI_DONE;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);
    cJSON* connipdir = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "connlist", connipdir);

    while (connected_dev != NULL) {
        conn_dev_link* conn_dev_tmp = connected_dev;
        cJSON* dir;
        dir = cJSON_CreateObject();
        cJSON_AddItemToArray(connipdir, dir);
        cJSON_AddStringToObject(dir, "connip", connected_dev->ip);
        cJSON_AddStringToObject(dir, "connmac", connected_dev->mac);
        connected_dev = connected_dev->next;
        free(conn_dev_tmp);
    }

    ESP_LOGD(TAG, "Connect ip data:%s\n", cJSON_Print(root));

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}


/*

*  Request: link_id=0&type=UDP&remote_ip=192.168.3.137&remote_port=8080&local_port=9090&udp_mode=1
*  	    link_id=0&type=TCP&remote_ip=192.168.3.137&remote_port=8080&keep_alive=1
*  Response: {state:1,error_code:500} or {state:0}
*
*/
int cgiConnIp(HttpdConnData* connData)
{
    int len;
    esp_at_arg at_arg;

    char link_id[10];
    char type[10];
    char remote_ip[50];
    char remote_port[10];
    char keep_alive[10];
    char udp_mode[10];
    char local_port[10];

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "linkid", link_id, sizeof(link_id));

    if (len == -1) {
        at_arg.cipstart.link_id = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.cipstart.link_id = atoi(link_id);
    }

    len = httpdFindArg(connData->post->buff, "type", type, sizeof(type));

    if (len == -1) {
        memset(at_arg.cipstart.type, '\0', sizeof(at_arg.cipstart.type));
        //at_arg.cipstart.type = NULL;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        //at_arg.cipstart.type = type;
        strncpy(at_arg.cipstart.type, type, sizeof(at_arg.cipstart.type));
    }

    len = httpdFindArg(connData->post->buff, "remoteip", remote_ip, sizeof(remote_ip));

    if (len == -1) {
        memset(at_arg.cipstart.remote_ip, '\0', sizeof(at_arg.cipstart.remote_ip));
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        strncpy(at_arg.cipstart.remote_ip, remote_ip, sizeof(at_arg.cipstart.remote_ip));
    }

    len = httpdFindArg(connData->post->buff, "remoteport", remote_port, sizeof(remote_port));

    if (len == -1) {
        at_arg.cipstart.remote_port = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.cipstart.remote_port = atoi(remote_port);
    }

    len = httpdFindArg(connData->post->buff, "keepalive", keep_alive, sizeof(keep_alive));

    if (len == -1) {
        at_arg.cipstart.keep_alive = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        if (strlen(keep_alive) > 0) {
            at_arg.cipstart.keep_alive = atoi(keep_alive);
        } else {
            at_arg.cipstart.keep_alive = -1;
        }
    }


    len = httpdFindArg(connData->post->buff, "localport", local_port, sizeof(local_port));

    if (len == -1) {
        at_arg.cipstart.local_port = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        if (strlen(local_port) > 0) {
            at_arg.cipstart.local_port = atoi(local_port);
        } else {
            at_arg.cipstart.local_port = -1;
        }
    }

    len = httpdFindArg(connData->post->buff, "udpmode", udp_mode, sizeof(udp_mode));

    if (len == -1) {
        at_arg.cipstart.udp_mode = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        if (strlen(udp_mode) > 0) {
            at_arg.cipstart.udp_mode = atoi(udp_mode);
        } else {
            at_arg.cipstart.udp_mode = -1;
        }
    }

    esp_err_t ret = esp_at_establish_connect(&at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}


int cgiCloseIp(HttpdConnData* connData)
{
    int len;

    int8_t link_id = 0;
    char connect_link_id[10];

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);

    len = httpdFindArg(connData->post->buff, "link_id", connect_link_id, sizeof(connect_link_id));

    if (len == -1) {
        link_id = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        link_id = atoi(connect_link_id);
    }

    esp_err_t ret = esp_at_close_connect(link_id);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

/*
* Request: NULL
*
* Response:
{
  "state": 0,
  "ip_stat": 3,
  "iplist": [
    {
      "link_id": 0,
      "type": "TCP",
      "remoteip": "192.168.3.137",
      "remoteport": 8080,
      "localport": 54514,
      "tetype": 0
    },
    {
      "link_id": 1,
      "type": "UDP",
      "remoteip": "192.168.3.137",
      "remoteport": 8080,
      "localport": 9090,
      "tetype": 0
    }
  ]
}
*/

int cgiGetIpStatus(HttpdConnData* connData)
{
    conn_stat* st = NULL;
    char* printed_json = NULL;

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    st = esp_at_get_connect_status();

    if (st == NULL) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "sta_interface_status is ：%d", st->interface_status);

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);

    cJSON_AddNumberToObject(root, "ip_stat", st->interface_status);

    if (st->interface_status != CREAT_TRAN || st->status_link == NULL) {
        ESP_LOGI(TAG, "No transmission");
        httpdStartResponse(connData, 200);
        //We are going to send some HTML.
        httpdHeader(connData, "Content-Type", "text/json");
        //No more headers.
        httpdEndHeaders(connData);
        printed_json = cJSON_Print(root);
        httpdSend(connData, printed_json, -1);
        cJSON_Delete(root);

        if (printed_json != NULL) {
            free(printed_json);
            printed_json = NULL;
        }

        return HTTPD_CGI_DONE;
    }

    cJSON* ipdir = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "iplist", ipdir);

    // parse ip status
    while (st->status_link != NULL) {
        conn_status_link* status_tmp = st->status_link;
        cJSON* dir;
        dir = cJSON_CreateObject();
        cJSON_AddItemToArray(ipdir, dir);
        cJSON_AddNumberToObject(dir, "linkid", st->status_link->link_id);

        cJSON_AddStringToObject(dir, "type", st->status_link->type);

        cJSON_AddStringToObject(dir, "remoteip", st->status_link->remote_ip);

        cJSON_AddNumberToObject(dir, "remoteport", st->status_link->remote_port);

        cJSON_AddNumberToObject(dir, "localport", st->status_link->local_port);

        cJSON_AddNumberToObject(dir, "tetype", st->status_link->tetype);
        st->status_link = st->status_link->next;
        free(status_tmp);
    }

    free(st);
    ESP_LOGD(TAG, "Connected ip data:%s\n", cJSON_Print(root));

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}


/*
*  Request: sta_ip=192.168.3.155&sta_gateway=192.168.3.1&sta_netmask=255.255.255.0
*  Response: NULL
*  Note: this command will crash after reboot, need to resolve
*/

int cgiSetStaIpInfo(HttpdConnData* connData)
{
    int len;
    esp_at_arg at_arg;

    char ip[50];
    char gateway[50];
    char netmask[50];


    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);

    len = httpdFindArg(connData->post->buff, "staip", ip, sizeof(ip));

    if (len == -1) {
        at_arg.ipinfo.ip = NULL;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.ipinfo.ip = ip;
    }

    len = httpdFindArg(connData->post->buff, "stagateway", gateway, sizeof(gateway));

    if (len == -1) {
        at_arg.ipinfo.gateway = NULL;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.ipinfo.gateway = gateway;
    }

    len = httpdFindArg(connData->post->buff, "sta_netmask", netmask, sizeof(netmask));

    if (len == -1) {
        at_arg.ipinfo.netmask = NULL;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.ipinfo.netmask = netmask;
    }

    if (at_arg.ipinfo.ip == NULL) {
        ESP_LOGE(TAG, "Error parameter");
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    esp_err_t ret = esp_at_set_ip(STATION, &at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

/*
* ap_ip=192.168.4.9&ap_gateway=192.168.4.1&ap_netmask=255.255.255.0
*/

int cgiSetApIpInfo(HttpdConnData* connData)
{
    int len;
    esp_at_arg at_arg;

    char ip[50];
    char gateway[50];
    char netmask[50];


    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "apip", ip, sizeof(ip));

    if (len == -1) {
        at_arg.ipinfo.ip = NULL;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.ipinfo.ip = ip;
    }

    len = httpdFindArg(connData->post->buff, "apgateway", gateway, sizeof(gateway));

    if (len == -1) {
        at_arg.ipinfo.gateway = NULL;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.ipinfo.gateway = gateway;
    }

    len = httpdFindArg(connData->post->buff, "ap_netmask", netmask, sizeof(netmask));

    if (len == -1) {
        at_arg.ipinfo.netmask = NULL;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.ipinfo.netmask = netmask;
    }

    if (at_arg.ipinfo.ip == NULL) {
        ESP_LOGE(TAG, "Error parameter");
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    esp_err_t ret = esp_at_set_ip(SOFTAP, &at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiSetStaMac(HttpdConnData* connData)
{
    int len;
    esp_at_arg arg;
    char mac[30] = {0};

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "mac", mac, sizeof(mac));

    if (len == -1) {
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        memcpy(arg.cipmac.mac, mac, sizeof(arg.cipmac.mac));
    }

    esp_err_t ret = esp_at_set_mac(STATION, &arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiSetApMac(HttpdConnData* connData)
{
    int len;
    esp_at_arg arg;
    char mac[30] = {0};

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "mac", mac, sizeof(mac));

    if (len == -1) {
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        memcpy(arg.cipmac.mac, mac, sizeof(arg.cipmac.mac));
    }

    esp_err_t ret = esp_at_set_mac(SOFTAP, &arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiShowDhcp(HttpdConnData* connData)
{
    esp_err_t ret;
    esp_at_arg at_arg;
    char* printed_json = NULL;

//    esp_dhcp_status dhcp_status;
    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    /*
        ret = esp_at_dhcp_status(QUERY, &dhcp_status); // Get dhcp status
        if(ret == ESP_ERR_INVALID_STATE){
            ESP_LOGE(TAG, "Error parameter\n");
            reponseError(connData, 500);
            return HTTPD_CGI_DONE;
        }

        ESP_LOGI(TAG, "dhcp status:%d\n", dhcp_status);
    */
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);

//    cJSON_AddNumberToObject(root,"dhcp_status",dhcp_status);


    ret = esp_at_set_dhcps(QUERY, &at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    cJSON_AddNumberToObject(root, "enable", at_arg.cwdhcps.enable);
    cJSON_AddNumberToObject(root, "lease", at_arg.cwdhcps.lease);
    cJSON_AddStringToObject(root, "startip", at_arg.cwdhcps.start_ip);
    cJSON_AddStringToObject(root, "endip", at_arg.cwdhcps.end_ip);


    ESP_LOGD(TAG, "DHCPS data:%s\n", cJSON_Print(root));

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}

/*
*  Request: dhcpclient=1&dhcpserver=1&lease=100&start_ip=192.168.4.33&end_ip=192.168.4.100
*/

int cgiSetDhcp(HttpdConnData* connData)
{
    esp_err_t ret;
    int len = 0;
    esp_at_arg at_arg;

    uint8_t dhcpclient = 0;
    uint8_t dhcpserver = 0;
    uint8_t dhcp_status = 0;
    char dhcp_client[5];
    char dhcp_server[5];

    char lease[10];
    char start_ip[50];
    char end_ip[50];

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);

    len = httpdFindArg(connData->post->buff, "dhcpclient", dhcp_client, sizeof(dhcp_client));

    if (len == -1) {
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    len = httpdFindArg(connData->post->buff, "dhcpserver", dhcp_server, sizeof(dhcp_server));

    if (len == -1) {
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    dhcpclient = atoi(dhcp_client);
    dhcpserver = atoi(dhcp_server);

    dhcp_status = dhcpclient | (dhcpserver << 1);
    ESP_LOGI(TAG, "DHCP status:%d", dhcp_status);

    ret = esp_at_dhcp_status(SET, (esp_dhcp_status*) &dhcp_status);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error arguments\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }


    if (dhcpserver == 1) {
        len = httpdFindArg(connData->post->buff, "lease", lease, sizeof(lease));

        if (len == -1) {
            reponseError(connData, 400);
            return HTTPD_CGI_DONE;
        }

        len = httpdFindArg(connData->post->buff, "startip", start_ip, sizeof(start_ip));

        if (len == -1) {
            reponseError(connData, 400);
            return HTTPD_CGI_DONE;
        }

        len = httpdFindArg(connData->post->buff, "endip", end_ip, sizeof(end_ip));

        if (len == -1) {
            reponseError(connData, 400);
            return HTTPD_CGI_DONE;
        }

        at_arg.cwdhcps.enable = 1;
        at_arg.cwdhcps.lease = atoi(lease);
        memcpy(at_arg.cwdhcps.start_ip, start_ip, sizeof(at_arg.cwdhcps.start_ip));
        memcpy(at_arg.cwdhcps.end_ip, end_ip, sizeof(at_arg.cwdhcps.end_ip));

    } else {
        at_arg.cwdhcps.enable = 0;
    }

    ret = esp_at_set_dhcps(SET, &at_arg); //

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

/*
*
* Response:{"sysram":16553}
*/
int getRamSize(HttpdConnData* connData)
{
    int sysram_size = 0;
    char* printed_json = NULL;

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    sysram_size = esp_at_getramsize();

    if (sysram_size == -1) {
        ESP_LOGE(TAG, "Internal error");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);
    cJSON_AddNumberToObject(root, "sysram", sysram_size);
    printed_json = cJSON_Print(root);

    //Generate the header
    //We want the header to start with HTTP code 200, which means the document is found.
    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);

    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}


int cgiping(HttpdConnData* connData)
{
    int len;
    char ping[30];
    int32_t ping_time = 0;
    char* printed_json = NULL;

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "ping", ping, sizeof(ping));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    ping_time = esp_at_ping(ping);

    if (ping_time < 0) {
        ESP_LOGE(TAG, "Internal error");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);
    cJSON_AddNumberToObject(root, "ping", ping_time);
    ESP_LOGD(TAG, "Post ramsize json data:%s", cJSON_Print(root));

    //Generate the header
    //We want the header to start with HTTP code 200, which means the document is found.
    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}

/*
*  Request: ssid=testhttpserver&pwd=12345678&chl=5&ecn=3&maxconn=3&ssidhidden=0
*  Response: NULL
*/

int cgiSetAp(HttpdConnData* connData)
{
    int len = 0;
    esp_at_arg at_arg;

    char ssid[128] = {0};
    char pwd[128] = {0};
    char chl[10] = {0};
    char ecn[10] = {0};
    char max_conn[10] = {0};
    char ssid_hidden[10] = {0};

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);

    len = httpdFindArg(connData->post->buff, "ssid", ssid, sizeof(ssid));

    if (len == -1) {
        memset(at_arg.cwsap.ssid, '\0', sizeof(at_arg.cwsap.ssid));
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        strncpy(at_arg.cwsap.ssid, ssid, sizeof(at_arg.cwsap.ssid));
    }

    len = httpdFindArg(connData->post->buff, "pwd", pwd, sizeof(pwd));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        memset(at_arg.cwsap.pwd, '\0', sizeof(at_arg.cwsap.pwd));
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        strncpy(at_arg.cwsap.pwd, pwd, sizeof(at_arg.cwsap.pwd));
    }

    len = httpdFindArg(connData->post->buff, "chl", chl, sizeof(chl));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        at_arg.cwsap.chl = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.cwsap.chl = atoi(chl);
    }


    len = httpdFindArg(connData->post->buff, "ecn", ecn, sizeof(ecn));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        at_arg.cwsap.ecn = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.cwsap.ecn = atoi(ecn);
    }

    len = httpdFindArg(connData->post->buff, "maxconn", max_conn, sizeof(max_conn));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        at_arg.cwsap.max_conn = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.cwsap.max_conn = atoi(max_conn);
    }

    len = httpdFindArg(connData->post->buff, "ssidhidden", ssid_hidden, sizeof(ssid_hidden));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        at_arg.cwsap.ssid_hidden = -1;
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.cwsap.ssid_hidden = atoi(ssid_hidden);
    }

    esp_err_t ret = esp_at_set_ap(SET, &at_arg);

    if (ret == ESP_ERR_INVALID_ARG) {
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    } else if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "command execution error");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiConfigApInfo(HttpdConnData* connData)
{
    esp_err_t ret;
    esp_at_arg at_arg;
    char* printed_json = NULL;

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ret = esp_at_set_ap(QUERY, &at_arg);

    if (ret == ESP_ERR_INVALID_ARG) {
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    } else if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "command execution error");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);
    cJSON_AddStringToObject(root, "ssid", at_arg.cwsap.ssid);
    cJSON_AddStringToObject(root, "pwd", at_arg.cwsap.pwd);
    cJSON_AddNumberToObject(root, "chl", at_arg.cwsap.chl);
    cJSON_AddNumberToObject(root, "ecn", at_arg.cwsap.ecn);
    cJSON_AddNumberToObject(root, "max_conn", at_arg.cwsap.max_conn);
    cJSON_AddNumberToObject(root, "ssid_hidden", at_arg.cwsap.ssid_hidden);

    ESP_LOGD(TAG, "Config AP info:%s\n", cJSON_Print(root));

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}

/*
*
*request:  ssid=HUAWEI001&pwd=12345678&bssid=0&autoconn=1
*
*/

int cgiConnAp(HttpdConnData* connData)
{
    esp_err_t ret;
    int len = 0;
    esp_at_arg at_arg;
    char ssid[128] = {0};
    char pwd[128] = {0};
    char bssid[128] = {0};
    char autoconn[10] = {0};

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        httpdStartResponse(connData, 406);  //http error code 'unacceptable'
        httpdEndHeaders(connData);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s\n", connData->post->buff);

    len = httpdFindArg(connData->post->buff, "ssid", ssid, sizeof(ssid));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        memset(at_arg.cwjap.ssid, '\0', sizeof(at_arg.cwjap.ssid));
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        strncpy(at_arg.cwjap.ssid, ssid, sizeof(at_arg.cwjap.ssid));
    }

    len = httpdFindArg(connData->post->buff, "pwd", pwd, sizeof(pwd));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        memset(at_arg.cwjap.pwd, '\0', sizeof(at_arg.cwjap.pwd));
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        strncpy(at_arg.cwjap.pwd, pwd, sizeof(at_arg.cwjap.pwd));
    }

    len = httpdFindArg(connData->post->buff, "bssid", bssid, sizeof(bssid));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        memset(at_arg.cwjap.bssid, '\0', sizeof(at_arg.cwjap.bssid));
    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        if (bssid[0] == '0') {
            memset(at_arg.cwjap.bssid, '\0', sizeof(at_arg.cwjap.bssid));
        } else {
            strncpy(at_arg.cwjap.bssid, bssid, sizeof(at_arg.cwjap.bssid));
        }
    }

    if (strlen(at_arg.cwjap.pwd) == 0 || strlen(at_arg.cwjap.ssid) == 0) {
        ESP_LOGE(TAG, "Error pwd or ssid\n");
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    ret = esp_at_join_ap(SET, &at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error pwd or ssid\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    len = httpdFindArg(connData->post->buff, "autoconn", autoconn, sizeof(autoconn));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        ESP_LOGE(TAG, "No autoconn");
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;

    } else {
        //If len isn't -1, the variable is found and is copied to the 'name' variable
        at_arg.autoconn.enable = atoi(autoconn);
    }

    if (at_arg.autoconn.enable == 0) {
        ret = esp_at_auto_connect(SET, &at_arg);

        if (ret == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "Error pwd or ssid\n");
            reponseError(connData, 500);
            return HTTPD_CGI_DONE;
        }

        ESP_LOGI(TAG, "Set auto conn %d", at_arg.autoconn.enable);
    }

    reponseOk(connData);
    ESP_LOGI(TAG, "Connect AP success");
    return HTTPD_CGI_DONE;
}

void wifiscanresult_cb(scan_ap_link* dev_head)
{
    ESP_LOGD(TAG, "Receive a wifi scan result\n");
    uint8_t count = 0;
    scan_ap_link* scan_info = dev_head;
    scan_ap_link* scan_tmp;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);

    cJSON* scandir = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "wifilist", scandir);

    while (scan_info) {
        scan_tmp = scan_info;
        ESP_LOGD(TAG, "scan info: ecn:%d, ssid:%s, rssi:%d, mac:%s, channel:%d\n", scan_info->ecn, scan_info->ssid, scan_info->rssi, scan_info->mac, scan_info->channel);

        if (count < MAX_SHOW_SCAN_AP) {           // Only show max number scan info
            cJSON* dir;
            dir = cJSON_CreateObject();

            cJSON_AddItemToArray(scandir, dir);

            cJSON_AddNumberToObject(dir, "ecn", scan_info->ecn);

            cJSON_AddStringToObject(dir, "ssid", scan_info->ssid);

            //cJSON_AddNumberToObject(dir, "rssi", scan_info->rssi);

            cJSON_AddStringToObject(dir, "mac", scan_info->mac);

            // cJSON_AddNumberToObject(dir, "channel", scan_info->channel);
            count ++;
        }

        scan_info = scan_info->next;
        free(scan_tmp);
    }
}

int cgiScanAp(HttpdConnData* connData)
{
    esp_err_t ret = -1;
    char* printed_json = NULL;

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ret = esp_at_wifi_scan(wifiscanresult_cb);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "command execution error");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGD(TAG, "WiFi scan data:%s\n", cJSON_Print(root));

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}


int cgiBleSet(HttpdConnData* connData)
{
    int len;
    esp_err_t ret;
    esp_at_arg at_arg;
    char init_mode[10];
    //char random_addr[50];
//    char device_name[50];
    char scan_type[10];
    char addr_type[10];
    char filter_policy[10];
    char scan_interval[10];
    char scan_window[10];

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "init_mode", init_mode, sizeof(init_mode));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    if (set_ble_mode(atoi(init_mode)) == -1) {
        ESP_LOGE(TAG, "Cannot change BLE init mode");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "BLE init success.");
    /*
        len = httpdFindArg(connData->post->buff, "devicename", device_name, sizeof(device_name));

        if (len != -1 && strlen(device_name) > 0) {
    	strncpy(at_arg.blename.device_name, device_name,sizeof(at_arg.blename.device_name));

            ret = esp_at_ble_name(SET, &at_arg);
            if(ret == ESP_ERR_INVALID_STATE){
                ESP_LOGE(TAG, "Error parameter\n");
                reponseError(connData, 500);
                return HTTPD_CGI_DONE;
            }
        }
    */
    len = httpdFindArg(connData->post->buff, "scantype", scan_type, sizeof(scan_type));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        ESP_LOGI(TAG, "No scan type");
    }

    len = httpdFindArg(connData->post->buff, "filterpolicy", filter_policy, sizeof(filter_policy));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        ESP_LOGI(TAG, "No filter policy");
    }

    len = httpdFindArg(connData->post->buff, "scaninterval", scan_interval, sizeof(scan_interval));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        ESP_LOGI(TAG, "No scan interval");
    }

    len = httpdFindArg(connData->post->buff, "scanwindow", scan_window, sizeof(scan_window));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        ESP_LOGI(TAG, "No scan window");
    }

    if (strlen(scan_type) > 0 && strlen(filter_policy) > 0 && strlen(scan_interval) > 0 && strlen(scan_window) > 0) {
        at_arg.blescanparam.scan_type = atoi(scan_type);
        at_arg.blescanparam.addr_type = atoi(addr_type);
        at_arg.blescanparam.filter_policy = atoi(filter_policy);
        at_arg.blescanparam.scan_interval = atoi(scan_interval);
        at_arg.blescanparam.scan_window = atoi(scan_window);

        ret = esp_at_ble_scan_param(SET, &at_arg);

        if (ret == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "Error parameter\n");
            reponseError(connData, 500);
            return HTTPD_CGI_DONE;
        }

        ESP_LOGI(TAG, "Set BLE scan para success.");
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiGetBleSet(HttpdConnData* connData)
{
    esp_err_t ret;
    esp_at_arg at_arg;
    char* printed_json = NULL;
//    int8_t init_mode;
//    char device_name[30] = {0};

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    /*
        ret = esp_at_ble_init(QUERY,&at_arg);
        if(ret == ESP_ERR_INVALID_STATE){
            ESP_LOGE(TAG, "Error parameter\n");
            reponseError(connData, 500);
            return HTTPD_CGI_DONE;
        }

        init_mode = at_arg.bleinit.init_mode;

        vTaskDelay(20/ portTICK_RATE_MS);    // wait some time

        ret = esp_at_ble_name(QUERY,&at_arg);
        if(ret == ESP_ERR_INVALID_STATE){
            ESP_LOGE(TAG, "Error parameter\n");
            reponseError(connData, 500);
            return HTTPD_CGI_DONE;
        }

        memcpy(device_name,at_arg.blename.device_name,sizeof(device_name));

        vTaskDelay(20/ portTICK_RATE_MS);    // wait some time
    */
    ret = esp_at_ble_scan_param(QUERY, &at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);
//    cJSON_AddNumberToObject(root,"initmode",init_mode);
//   cJSON_AddStringToObject(root,"devicename",device_name);

    cJSON_AddNumberToObject(root, "scantype", at_arg.blescanparam.scan_type);
    cJSON_AddNumberToObject(root, "addrtype", at_arg.blescanparam.addr_type);
    cJSON_AddNumberToObject(root, "filterpolicy", at_arg.blescanparam.filter_policy);
    cJSON_AddNumberToObject(root, "scaninterval", at_arg.blescanparam.scan_interval);
    cJSON_AddNumberToObject(root, "scanwindow", at_arg.blescanparam.scan_window);

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}

int cgiGetBleAdvSet(HttpdConnData* connData)
{
    esp_err_t ret;
    esp_at_arg at_arg;
    char* printed_json = NULL;

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ret = esp_at_ble_adv_param(QUERY, &at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);
    cJSON_AddNumberToObject(root, "advintmin", at_arg.bleadvparam.adv_int_min);

    cJSON_AddNumberToObject(root, "advintmax", at_arg.bleadvparam.adv_int_max);
    cJSON_AddNumberToObject(root, "advtype", at_arg.bleadvparam.adv_type);
    cJSON_AddNumberToObject(root, "addrtype", at_arg.bleadvparam.addr_type);
    cJSON_AddNumberToObject(root, "channel", at_arg.bleadvparam.channel);

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}

void blescanresult_cb(ble_scan_info* dev_head)
{
    ble_scan_info* scan_info = dev_head;
    ble_scan_info* scan_tmp;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);

    cJSON* scandir = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "blelist", scandir);

    while (scan_info) {
        scan_tmp = scan_info;
        ESP_LOGD(TAG, "scan info:addr: %s, rssi:%d, adv_data:%s, scan_rsp_data:%s\n", scan_info->addr, scan_info->rssi, scan_info->adv_data, scan_info->scan_rsp_data);
        cJSON* dir;
        dir = cJSON_CreateObject();

        cJSON_AddItemToArray(scandir, dir);

        cJSON_AddStringToObject(dir, "addr", scan_info->addr);

        cJSON_AddNumberToObject(dir, "rssi", scan_info->rssi);

        cJSON_AddStringToObject(dir, "adv_data", scan_info->adv_data);

        cJSON_AddStringToObject(dir, "scan_rsp_data", scan_info->scan_rsp_data);

        scan_info = scan_info->next;

        if (scan_tmp != dev_head) {
            free(scan_tmp);
        }

        if (scan_info == dev_head) {
            ESP_LOGI(TAG, "This is the deadline");
            break;
        }
    }

    free(dev_head);
}

int cgiBleScan(HttpdConnData* connData)
{
    int len;
    esp_err_t ret;
    esp_at_arg at_arg;
    char enable[10];
    char interval[10];
    char* printed_json = NULL;
    extern cJSON* root;

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);

    if (set_ble_mode(1) == -1) {
        ESP_LOGE(TAG, "Cannot change BLE init mode");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }


    len = httpdFindArg(connData->post->buff, "enable", enable, sizeof(enable));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    at_arg.blescan.enable = atoi(enable);

    len = httpdFindArg(connData->post->buff, "interval", interval, sizeof(interval));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        at_arg.blescan.interval = -1;
    } else {
        at_arg.blescan.interval = atoi(interval);
    }

    vTaskDelay(50 / portTICK_RATE_MS);    // wait some time

    ret = esp_at_ble_scan(&at_arg, blescanresult_cb);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "BLE scan error");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGD(TAG, "Ble dev data:%s\n", cJSON_Print(root));

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}

int cgiBleConn(HttpdConnData* connData)
{
    int len;
    esp_at_arg at_arg;
    char conn_index[5];
    char remote_address[30];

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "connindex", conn_index, sizeof(conn_index));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    at_arg.bleconn.conn_index = atoi(conn_index);

    len = httpdFindArg(connData->post->buff, "remoteaddress", remote_address, sizeof(remote_address));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    strncpy(at_arg.bleconn.remote_address, remote_address, sizeof(at_arg.bleconn.remote_address));
    esp_err_t ret = esp_at_ble_connect(&at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "connect fail");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiGetBleConn(HttpdConnData* connData)
{
    char* printed_json = NULL;
    ble_conn_dev* conn_dev_link = NULL;

    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    if (connData->requestType != HTTPD_METHOD_GET) {
        //Sorry, we only accept GET requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    conn_dev_link = esp_at_get_ble_connect();

    if (conn_dev_link == NULL) {
        ESP_LOGI(TAG, "no connect");
        reponseOk(connData);
        return HTTPD_CGI_DONE;
    }

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "state", 0);

    cJSON* conndir = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "connlist", conndir);

    while (conn_dev_link) {
        ble_conn_dev* dev_tmp = conn_dev_link;
        cJSON* dir;
        dir = cJSON_CreateObject();

        cJSON_AddItemToArray(conndir, dir);

        cJSON_AddNumberToObject(dir, "connid", conn_dev_link->conn_index);

        cJSON_AddStringToObject(dir, "addr", conn_dev_link->addr);

        conn_dev_link = conn_dev_link->next;
        free(dev_tmp);
    }

    ESP_LOGD(TAG, "Ble connect info:%s\n", cJSON_Print(root));

    httpdStartResponse(connData, 200);
    //We are going to send some HTML.
    httpdHeader(connData, "Content-Type", "text/json");
    //No more headers.
    httpdEndHeaders(connData);
    printed_json = cJSON_Print(root);
    httpdSend(connData, printed_json, -1);
    cJSON_Delete(root);

    if (printed_json != NULL) {
        free(printed_json);
        printed_json = NULL;
    }

    return HTTPD_CGI_DONE;
}

int cgiBleCloseConn(HttpdConnData* connData)
{
    int len;
    char conn_index[5];
    int8_t conn_id = -1;

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "conn_index", conn_index, sizeof(conn_index));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    conn_id = atoi(conn_index);

    esp_err_t ret = esp_at_ble_close_connect(conn_id);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "connect fail");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiBleAdvSet(HttpdConnData* connData)
{
    int len;
    esp_err_t ret;
    esp_at_arg at_arg;
    char adv_int_min[10];
    char adv_int_max[10];
    char adv_type[10];
    char addr_type[10];
    char channel[10];

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "advintmin", adv_int_min, sizeof(adv_int_min));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    len = httpdFindArg(connData->post->buff, "advintmax", adv_int_max, sizeof(adv_int_max));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    len = httpdFindArg(connData->post->buff, "advtype", adv_type, sizeof(adv_type));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    len = httpdFindArg(connData->post->buff, "addrtype", addr_type, sizeof(addr_type));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    len = httpdFindArg(connData->post->buff, "channel", channel, sizeof(channel));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    if (set_ble_mode(2) == -1) {
        ESP_LOGE(TAG, "Cannot change BLE init mode");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    vTaskDelay(100 / portTICK_RATE_MS);    // wait some time

    at_arg.bleadvparam.adv_int_min = atoi(adv_int_min);
    at_arg.bleadvparam.adv_int_max = atoi(adv_int_max);
    at_arg.bleadvparam.adv_type = atoi(adv_type);
    at_arg.bleadvparam.addr_type = atoi(addr_type);
    at_arg.bleadvparam.channel = atoi(channel);

    ret = esp_at_ble_adv_param(SET, &at_arg);

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiBleAdvStart(HttpdConnData* connData)
{
    int len;
    esp_err_t ret;
    char adv_data[32];

    if (connData->requestType != HTTPD_METHOD_POST) {
        //Sorry, we only accept POST requests.
        reponseError(connData, 406);
        return HTTPD_CGI_DONE;
    }

    ESP_LOGI(TAG, "Post data:%s", connData->post->buff);
    len = httpdFindArg(connData->post->buff, "advdata", adv_data, sizeof(adv_data));

    if (len == -1) {
        //If the result of httpdFindArg is -1, the variable isn't found in the data.
        reponseError(connData, 400);
        return HTTPD_CGI_DONE;
    }

    if (set_ble_mode(2) == -1) {
        ESP_LOGE(TAG, "Cannot change BLE init mode");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    vTaskDelay(100 / portTICK_RATE_MS);    // wait some time

    if ((strlen(adv_data) > 0) && (strlen(adv_data) < 31)) {
        ret = esp_at_ble_set_adv_data(adv_data);

        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error set adv data");
            reponseError(connData, 500);
            return HTTPD_CGI_DONE;
        }
    }

    vTaskDelay(100 / portTICK_RATE_MS);    // wait some time

    ret = esp_at_ble_adv_start();

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}

int cgiBleAdvStop(HttpdConnData* connData)
{
    esp_err_t ret;
    ret = esp_at_ble_adv_stop();

    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Error parameter\n");
        reponseError(connData, 500);
        return HTTPD_CGI_DONE;
    }

    reponseOk(connData);
    return HTTPD_CGI_DONE;
}
