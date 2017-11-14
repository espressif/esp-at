/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
#include <string.h>

#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "freertos/timers.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"

#include "tcpip_adapter.h"

#include "esp_at.h"

#ifdef CONFIG_AT_OTA_SSL_SUPPORT
#include "openssl/ssl.h"
#endif
#include "at_upgrade.h"
#define TEXT_BUFFSIZE 1024

#define UPGRADE_FRAME  "{\"path\": \"/v1/messages/\", \"method\": \"POST\", \"meta\": {\"Authorization\": \"token %s\"},\
\"get\":{\"action\":\"%s\"},\"body\":{\"pre_rom_version\":\"%s\",\"rom_version\":\"%s\"}}\n"

#define pheadbuffer "Connection: keep-alive\r\n\
Cache-Control: no-cache\r\n\
User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36 \r\n\
Accept: */*\r\n\
Authorization: token %s\r\n\
Accept-Encoding: gzip,deflate,sdch\r\n\
Accept-Language: zh-CN,zh;q=0.8\r\n\r\n"

#define ESP_AT_OTA_TIMEOUT_MS               (60*3*1000)

static xTimerHandle esp_at_ota_timeout_timer = NULL;
static bool esp_at_ota_timeout_flag = false;
static int esp_at_ota_socket_id = -1;

#define ESP_AT_OTA_DEBUG  printf

static void esp_at_ota_timeout_callback( TimerHandle_t xTimer )
{
    ESP_AT_OTA_DEBUG("ota timeout!\r\n");
    esp_at_ota_timeout_flag = true;
    if (esp_at_ota_socket_id >= 0) {
        close(esp_at_ota_socket_id);
        esp_at_ota_socket_id = -1;
    }
}

bool esp_at_upgrade_process(int32_t ota_mode,uint8_t* version)
{
    bool pkg_body_start = false;
    struct sockaddr_in sock_info;
    ip_addr_t ip_address;
    struct hostent* hptr = NULL;
    uint8_t* http_request = NULL;
    uint8_t* data_buffer = NULL;
    uint8_t* pStr = NULL;
    esp_partition_t* partition_ptr = NULL;
    esp_partition_t partition;
    esp_partition_t* next_partition = NULL;
    esp_ota_handle_t out_handle = 0;
    int buff_len = 0;
    int sockopt = 1;
    int total_len = 0;
    int recv_len = 0;
    bool ret = false;
    int result = -1;
    char* server_ip = NULL;
    uint16_t server_port = 0;
    char* ota_key = NULL;
    
#ifdef CONFIG_AT_OTA_SSL_SUPPORT
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
#endif

    ESP_AT_OTA_DEBUG("ota_mode:%d\r\n",ota_mode);
    if (ota_mode == ESP_AT_OTA_MODE_NORMAL) {
        server_ip = CONFIG_AT_OTA_SERVER_IP;
        server_port = CONFIG_AT_OTA_SERVER_PORT;
        ota_key = CONFIG_AT_OTA_TOKEN_KEY;
    }
#ifdef CONFIG_AT_OTA_SSL_SUPPORT
    else if (ota_mode == ESP_AT_OTA_MODE_SSL) {
        server_ip = CONFIG_AT_OTA_SSL_SERVER_IP;
        server_port = CONFIG_AT_OTA_SSL_SERVER_PORT;
        ota_key = CONFIG_AT_OTA_SSL_TOKEN_KEY;
    }
#endif
    else {
        return ret;
    }
    
    
    if (esp_at_ota_timeout_timer != NULL) {
        xTimerStop(esp_at_ota_timeout_timer,portMAX_DELAY);
        xTimerDelete(esp_at_ota_timeout_timer,portMAX_DELAY);
        esp_at_ota_timeout_timer = NULL;
    }
    esp_at_ota_timeout_flag = false;
    esp_at_ota_timeout_timer = xTimerCreate("ota_timer",
                ESP_AT_OTA_TIMEOUT_MS/portTICK_PERIOD_MS,
                pdFAIL,
                NULL,
                esp_at_ota_timeout_callback);
    xTimerStart(esp_at_ota_timeout_timer,portMAX_DELAY);
    ip_address.u_addr.ip4.addr = inet_addr(server_ip);

    if ((ip_address.u_addr.ip4.addr == IPADDR_NONE) && (strcmp(server_ip,"255.255.255.255") != 0)) {
        if((hptr = gethostbyname(server_ip)) == NULL)
        {
            ESP_AT_OTA_DEBUG("gethostbyname fail\r\n");
            goto OTA_ERROR;
        }
        ip_address = *(ip_addr_t*)hptr->h_addr_list[0];
    }

    // set connect info
    memset(&sock_info, 0, sizeof(struct sockaddr_in));
    sock_info.sin_family = AF_INET;
    sock_info.sin_addr.s_addr = ip_address.u_addr.ip4.addr;
    sock_info.sin_port = htons(server_port);
    esp_at_port_write_data((uint8_t*)"+CIPUPDATE:1\r\n",strlen("+CIPUPDATE:1\r\n"));

    http_request = (uint8_t*)malloc(TEXT_BUFFSIZE);
    if (http_request == NULL) {
        goto OTA_ERROR;
    }
    data_buffer = (uint8_t*)malloc(TEXT_BUFFSIZE);
    if (data_buffer == NULL) {
        goto OTA_ERROR;
    }

    if (version == NULL) {
        esp_at_ota_socket_id = socket(AF_INET, SOCK_STREAM, 0);
        if (esp_at_ota_socket_id < 0) {
            goto OTA_ERROR;
        }

        setsockopt(esp_at_ota_socket_id, SOL_SOCKET, SO_REUSEADDR,&sockopt, sizeof(sockopt));
        // connect to http server
        if (connect(esp_at_ota_socket_id, (struct sockaddr*)&sock_info, sizeof(sock_info)) < 0)
        {
            ESP_AT_OTA_DEBUG("connect to server failed!\r\n");
            goto OTA_ERROR;
        }

    #ifdef CONFIG_AT_OTA_SSL_SUPPORT
        if (ota_mode == ESP_AT_OTA_MODE_SSL) {
            ctx = SSL_CTX_new(TLSv1_2_client_method());
            if (!ctx) {
                ESP_AT_OTA_DEBUG("ctx fail\r\n");
                goto OTA_ERROR;
            }
            ssl = SSL_new(ctx);
            SSL_CTX_free(ctx);
            ctx = NULL;
            if (!ssl) {
                ESP_AT_OTA_DEBUG("ssl fail\r\n");
                goto OTA_ERROR;
            }

            SSL_set_fd(ssl, esp_at_ota_socket_id);

            if(!SSL_connect(ssl)) {
                ESP_AT_OTA_DEBUG("ssl connect fail\r\n");
                goto OTA_ERROR;
            }
        }
    #endif

        esp_at_port_write_data((uint8_t*)"+CIPUPDATE:2\r\n",strlen("+CIPUPDATE:2\r\n"));

        snprintf((char*)http_request,TEXT_BUFFSIZE,"GET /v1/device/rom/?is_format_simple=true HTTP/1.0\r\nHost: "IPSTR":%d\r\n"pheadbuffer"",
                 IP2STR(&ip_address.u_addr.ip4),
                 server_port, ota_key);

        printf("http request length %d bytes\r\n",strlen((char*)http_request));
        /*send GET request to http server*/
        result = -1;
        if (ota_mode == ESP_AT_OTA_MODE_NORMAL) {
            result = write(esp_at_ota_socket_id, http_request, strlen((char*)http_request));
        }
    #ifdef CONFIG_AT_OTA_SSL_SUPPORT
        else if (ota_mode == ESP_AT_OTA_MODE_SSL) {
            result = SSL_write(ssl,http_request, strlen((char*)http_request));
        }
    #endif

        if(result != strlen((char*)http_request))
        {
            ESP_AT_OTA_DEBUG("send GET request to server failed\r\n");
            goto OTA_ERROR;
        }

        memset(data_buffer,0x0,TEXT_BUFFSIZE);

        result = -1;
        if (ota_mode == ESP_AT_OTA_MODE_NORMAL) {
            result = read(esp_at_ota_socket_id, data_buffer, TEXT_BUFFSIZE);
        }
    #ifdef CONFIG_AT_OTA_SSL_SUPPORT
        else if (ota_mode == ESP_AT_OTA_MODE_SSL) {
            result = SSL_read(ssl,data_buffer, TEXT_BUFFSIZE);
        }

        if (ssl != NULL) {
            SSL_shutdown(ssl);
            SSL_free(ssl);
            ssl = NULL;
        }
    #endif
        close(esp_at_ota_socket_id);
        esp_at_ota_socket_id = -1;

        if (result < 0) {
            ESP_AT_OTA_DEBUG("recv data from server failed!\r\n");
            goto OTA_ERROR;
        }
        esp_at_port_write_data((uint8_t*)"+CIPUPDATE:3\r\n",strlen("+CIPUPDATE:3\r\n"));
        pStr = (uint8_t*)strstr((char*)data_buffer,"rom_version\": ");
        if (pStr == NULL) {
            ESP_AT_OTA_DEBUG("rom_version error!\r\n");
            goto OTA_ERROR;
        }
        pStr += strlen("rom_version\": \"");
        version = pStr;
    
        pStr = (uint8_t*)strstr((char*)version,"\",");
        if (pStr == NULL) {
            ESP_AT_OTA_DEBUG("rom_version tail error!\r\n");
            goto OTA_ERROR;
        }
        *pStr = '\0';
    }
    printf("version:%s\r\n",version);
    snprintf((char*)http_request,TEXT_BUFFSIZE,
        "GET /v1/device/rom/?action=download_rom&version=%s&filename=ota.bin HTTP/1.1\r\nHost: "IPSTR":%d\r\n"pheadbuffer"",
        (char*)version, IP2STR(&ip_address.u_addr.ip4),
        server_port, ota_key);

    // search ota partition
    partition_ptr = (esp_partition_t*)esp_ota_get_boot_partition();
    if (partition_ptr == NULL) {
        ESP_AT_OTA_DEBUG("boot partition NULL!\r\n");
        goto OTA_ERROR;
    }
    if (partition_ptr->type != ESP_PARTITION_TYPE_APP)
    {
        ESP_AT_OTA_DEBUG("esp_current_partition->type != ESP_PARTITION_TYPE_APP\r\n");
        goto OTA_ERROR;
    }

    if (partition_ptr->subtype == ESP_PARTITION_SUBTYPE_APP_FACTORY) {
        partition.subtype = ESP_PARTITION_SUBTYPE_APP_OTA_0;
    } else {
        next_partition = esp_ota_get_next_update_partition(partition_ptr);

        if (next_partition) {
            partition.subtype = next_partition->subtype;
        } else {
            partition.subtype = ESP_PARTITION_SUBTYPE_APP_OTA_0;
        }
    }

    partition.type = ESP_PARTITION_TYPE_APP;

    partition_ptr = (esp_partition_t*)esp_partition_find_first(partition.type,partition.subtype,NULL);
    if (partition_ptr == NULL) {
        ESP_AT_OTA_DEBUG("partition NULL!\r\n");
        goto OTA_ERROR;
    }

    memcpy(&partition,partition_ptr,sizeof(esp_partition_t));
    if(esp_ota_begin(&partition, OTA_SIZE_UNKNOWN, &out_handle) != ESP_OK)
    {
        ESP_AT_OTA_DEBUG("esp_ota_begin failed!\r\n");
        goto OTA_ERROR;
    }

    esp_at_ota_socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (esp_at_ota_socket_id < 0) {
       goto OTA_ERROR;
    }
    setsockopt(esp_at_ota_socket_id, SOL_SOCKET, SO_REUSEADDR,&sockopt, sizeof(sockopt));
    if (connect(esp_at_ota_socket_id, (struct sockaddr*)&sock_info, sizeof(sock_info)) < 0) {
        ESP_AT_OTA_DEBUG("connect to server2 failed!\r\n");
        goto OTA_ERROR;
    }
#ifdef CONFIG_AT_OTA_SSL_SUPPORT
    if (ota_mode == ESP_AT_OTA_MODE_SSL) {
        ctx = SSL_CTX_new(TLSv1_1_client_method());
        if (!ctx) {
            ESP_AT_OTA_DEBUG("ctx fail\r\n");
            goto OTA_ERROR;
        }
        ssl = SSL_new(ctx);
        SSL_CTX_free(ctx);
        ctx = NULL;
        if (!ssl) {
            ESP_AT_OTA_DEBUG("ssl fail\r\n");
            goto OTA_ERROR;
        }

        SSL_set_fd(ssl, esp_at_ota_socket_id);

        if(!SSL_connect(ssl)) {
            ESP_AT_OTA_DEBUG("ssl connect fail\r\n");
            goto OTA_ERROR;
        }
    }
#endif


    
    result = -1;
    if (ota_mode == ESP_AT_OTA_MODE_NORMAL) {
        result = write(esp_at_ota_socket_id, http_request, strlen((char*)http_request));
    }
#ifdef CONFIG_AT_OTA_SSL_SUPPORT
    else if (ota_mode == ESP_AT_OTA_MODE_SSL) {
        result = SSL_write(ssl,http_request, strlen((char*)http_request));
    }
#endif

    if(result != strlen((char*)http_request))
    {
        ESP_AT_OTA_DEBUG("send GET bin to server failed\r\n");
        goto OTA_ERROR;
    }
    /*deal with all receive packet*/
    for (;;)
    {
        memset(data_buffer, 0x0, TEXT_BUFFSIZE);
        
        buff_len = -1;
        if (ota_mode == ESP_AT_OTA_MODE_NORMAL) {
            buff_len = read(esp_at_ota_socket_id, data_buffer, TEXT_BUFFSIZE);
        }
#ifdef CONFIG_AT_OTA_SSL_SUPPORT
        else if (ota_mode == ESP_AT_OTA_MODE_SSL) {
            buff_len = SSL_read(ssl,data_buffer, TEXT_BUFFSIZE);
        }
#endif
        if (buff_len < 0) {
            ESP_AT_OTA_DEBUG("receive data error!\r\n");
            goto OTA_ERROR;
        } else if (buff_len > 0 && !pkg_body_start) {
            // search "\r\n\r\n"
            pStr = (uint8_t*)strstr((char*)data_buffer,"Content-Length: ");
            if (pStr == NULL) {
                break;
            }
            pStr += strlen("Content-Length: ");
            total_len = atoi((char*)pStr);
            ESP_AT_OTA_DEBUG("total_len=%d!\r\n",total_len);
            pStr = (uint8_t*)strstr((char*)data_buffer,"\r\n\r\n");
            if (pStr) {
                pkg_body_start = true;
                pStr += 4; // skip "\r\n"
                if (pStr[0] != 0xE9) {
                    ESP_AT_OTA_DEBUG("OTA Write Header format Check Failed! first byte is %02x\r\n", pStr[0]);
                    goto OTA_ERROR;
                }
                // pStr += 2;
                buff_len -= (pStr - data_buffer);
                if(esp_ota_write(out_handle, (const void*)pStr, buff_len) != ESP_OK)
                {
                    ESP_AT_OTA_DEBUG("esp_ota_write failed!\r\n");
                    goto OTA_ERROR;
                }

                recv_len += buff_len;
            }
        } else if (buff_len > 0 && pkg_body_start) {
            if(esp_ota_write( out_handle, (const void*)data_buffer, buff_len) != ESP_OK) {
                ESP_AT_OTA_DEBUG("esp_ota_write failed!\r\n");
                goto OTA_ERROR;
            }

            recv_len += buff_len;
        } else if (buff_len == 0) {
            ESP_AT_OTA_DEBUG("receive all packet over!\r\n");
            if (recv_len != total_len) {
                goto OTA_ERROR;
            }
        } else {
            ESP_AT_OTA_DEBUG("Warning: uncontolled event!\r\n");
        }
        ESP_AT_OTA_DEBUG("total_len=%d(%d), %0.1f%%!\r\n",total_len,recv_len,(recv_len*1.0)*100/total_len);
        if (recv_len == total_len) {
            break;
        }
    }

    if(esp_ota_end(out_handle) != ESP_OK)
    {
        ESP_AT_OTA_DEBUG("esp_ota_end failed!\r\n");
        goto OTA_ERROR;
    }

    if(esp_ota_set_boot_partition(&partition) != ESP_OK)
    {
        ESP_AT_OTA_DEBUG("esp_ota_set_boot_partition failed!\r\n");
        goto OTA_ERROR;
    }
    esp_at_port_write_data((uint8_t*)"+CIPUPDATE:4\r\n",strlen("+CIPUPDATE:4\r\n"));

    ret = true;
OTA_ERROR:
    if (esp_at_ota_timeout_timer != NULL) {
        xTimerStop(esp_at_ota_timeout_timer,portMAX_DELAY);
        xTimerDelete(esp_at_ota_timeout_timer,portMAX_DELAY);
        esp_at_ota_timeout_timer = NULL;
    }
    
    if (esp_at_ota_socket_id >= 0) {
        close(esp_at_ota_socket_id);
        esp_at_ota_socket_id = -1;
    }
    
    if (http_request) {
        free(http_request);
        http_request = NULL;
    }

    if (data_buffer) {
        free(data_buffer);
        data_buffer = NULL;
    }
    
#ifdef CONFIG_AT_OTA_SSL_SUPPORT
    if (ssl != NULL) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        ssl = NULL;
    }
#endif
    return ret;
}

