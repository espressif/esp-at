/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#ifdef CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE
#include "at_web_dns_server.h"

#define MAX(a, b)                                     ((a) > (b) ? (a) : (b))

#define AT_WEB_DNS_PORT                                53
#define AT_WEB_DNS_MAX_LEN                             200

#define AT_WEB_QR_FLAG                                 (1 << 7)
#define AT_WEB_OPCODE_MASK                             0x7800
#define AT_WEB_QD_TYPE_A                               0x0001
#define AT_WEB_ANS_TTL_SEC                             300
#define AT_WEB_LOCALHOST_PORT                          (5001)
#define AT_WEB_TASK_EXIT_STR                           ("exit")
#define CAPTIVE_PORTAL_DNS_SERVER_TASK_PRIORITY        5

#define AT_WEB_TASK_EXIT_SUCCESS_BIT                   BIT0

static const char *TAG = "dns_redirect_server";
static int s_dns_server_socket_fd = -1;
static int s_dns_server_localhost_fd = -1;
static struct sockaddr_in s_dest_addr = { 0 };
static EventGroupHandle_t s_web_dns_event_group = NULL;

typedef struct __attribute__((__packed__))
{
    uint16_t id; // identification number
    uint16_t flags;
    uint16_t qd_count; // number of question entries
    uint16_t an_count; // number of answer entries
    uint16_t ns_count; // number of authority entries
    uint16_t ar_count; // number of resource entries
} dns_header_t;

typedef struct {
    uint16_t type;
    uint16_t class;
} dns_question_t;

typedef struct __attribute__((__packed__))
{
    uint16_t ptr_offset;
    uint16_t type;
    uint16_t class;
    uint32_t ttl;
    uint16_t addr_len;
    uint32_t ip_addr;
} dns_answer_t;

/**
 * @brief close socket created in dns_server_task
 *
 */
static void web_dns_socket_close(void)
{
    if (s_dns_server_localhost_fd != -1) {
        shutdown(s_dns_server_localhost_fd, 0);
        close(s_dns_server_localhost_fd);
        s_dns_server_localhost_fd = -1;
    }

    if (s_dns_server_socket_fd != -1) {
        shutdown(s_dns_server_socket_fd, 0);
        close(s_dns_server_socket_fd);
        s_dns_server_socket_fd = -1;
    }
}

/**
 * @brief localhost udp is only used to exit dns_server_task
 *
 */
static esp_err_t localhost_udp_create(void)
{
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_UDP;

    memset(&s_dest_addr, 0, sizeof(struct sockaddr));

    s_dest_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    s_dest_addr.sin_family = AF_INET;
    s_dest_addr.sin_port = htons(AT_WEB_LOCALHOST_PORT);

    s_dns_server_localhost_fd = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (s_dns_server_localhost_fd < 0) {
        ESP_LOGE(TAG, "Unable to create localhost socket: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Localhost socket created");

    if (bind(s_dns_server_localhost_fd, (struct sockaddr *)&s_dest_addr, sizeof(s_dest_addr)) < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        web_dns_socket_close();
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Localhost socket bound, port %d", AT_WEB_LOCALHOST_PORT);

    return ESP_OK;
}

/* Parse the name from the packet from the dns name format to a regular .-seperated name
   returns the pointer to the next part of the packet
*/
static char *parse_dns_name(char *raw_name, char *parsed_name, size_t parsed_name_max_len)
{

    char *label = raw_name;
    char *name_itr = parsed_name;
    int name_len = 0;
    int sub_name_len = 0;
    do {
        sub_name_len = *label;
        // Len + 1 since we are adding '.'
        name_len += (sub_name_len + 1);
        if (name_len > parsed_name_max_len) {
            return NULL;
        }

        // Copy the sub name that follows the the label
        memcpy(name_itr, label + 1, sub_name_len);
        name_itr[sub_name_len] = '.';
        name_itr += (sub_name_len + 1);

        label += sub_name_len + 1;

    } while (*label != 0);

    // Terminate the final string, replacing the last '.'
    parsed_name[name_len - 1] = '\0';

    // Return pointer to first char after the name
    return label + 1;
}

/* Parses the DNS request and prepares a DNS response with the IP of the softAP */
static int parse_dns_request(char *req, size_t req_len, char *dns_reply, size_t dns_reply_max_len)
{
    memset(dns_reply, 0, dns_reply_max_len);

    if (req_len > dns_reply_max_len) {
        return -1;
    }

    // Prepare the reply
    memcpy(dns_reply, req, req_len);

    // Endianess of NW packet different from chip
    dns_header_t *header = (dns_header_t*)dns_reply;
    ESP_LOGD(TAG, "DNS req with header id: 0x%X, flags: 0x%X, qd_count: %d", ntohs(header->id), ntohs(header->flags), ntohs(header->qd_count));

    if ((header->flags & AT_WEB_OPCODE_MASK) != 0) {
        // Not a standard query
        return 0;
    }

    // set question response flag
    header->flags |= AT_WEB_QR_FLAG;

    uint16_t qd_count = ntohs(header->qd_count);
    header->an_count  = htons(qd_count);

    int reply_len = qd_count * sizeof(dns_answer_t) + req_len;
    if (reply_len > dns_reply_max_len) {
        return -1;
    }

    // Pointer to current answer and question
    char *cur_ans_ptr = dns_reply + req_len;
    char *cur_qd_ptr  = dns_reply + sizeof(dns_header_t);

    char name[128] = {0};
    /* Repond to all questions with the ESP's IP address */
    for (int i = 0; i < qd_count; i++) {
        char *name_end_ptr = parse_dns_name(cur_qd_ptr, name, sizeof(name));
        if (name_end_ptr == NULL) {
            ESP_LOGE(TAG, "Failed to parse DNS question: %s", cur_qd_ptr);
            return -1;
        }

        dns_question_t *question = (dns_question_t*)(name_end_ptr);
        uint16_t qd_type = ntohs(question->type);
        uint16_t qd_class = ntohs(question->class);

        ESP_LOGD(TAG, "Received type: %u, class: %u question for name: %s", qd_type, qd_class, name);

        if (qd_type == AT_WEB_QD_TYPE_A) {
            dns_answer_t *answer = (dns_answer_t*)cur_ans_ptr;

            answer->ptr_offset = htons(0xC000 | (cur_qd_ptr - dns_reply));

            answer->type = htons(qd_type);
            answer->class = htons(qd_class);
            answer->ttl = htonl(AT_WEB_ANS_TTL_SEC);

            esp_netif_ip_info_t ip_info;
            esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"), &ip_info);
            ESP_LOGD(TAG, "Answer with PTR offset: 0x%X and IP 0x%X", ntohs(answer->ptr_offset), ip_info.ip.addr);

            answer->addr_len = htons(sizeof(ip_info.ip.addr));
            answer->ip_addr = ip_info.ip.addr;
        }
    }
    return reply_len;
}

/* Sets up a socket and listen for DNS queries,
    replies to all type A queries with the IP of the softAP
 */
void dns_server_task(void *pvParameters)
{
    char rx_buffer[128] = {0};
    char addr_str[128] = {0};
    int addr_family;
    int ip_protocol;

    while (1) {
        if (localhost_udp_create() != ESP_OK) {
            break;
        }

        struct sockaddr_in dest_addr = {0};
        dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(AT_WEB_DNS_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

        s_dns_server_socket_fd = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (s_dns_server_socket_fd < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

        int err = bind(s_dns_server_socket_fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket bound, port %d", AT_WEB_DNS_PORT);

        while (1) {
            struct sockaddr_in6 source_addr = {0}; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);

            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(s_dns_server_socket_fd, &rfds);
            FD_SET(s_dns_server_localhost_fd, &rfds);

            int s = select(MAX(s_dns_server_socket_fd, s_dns_server_localhost_fd) + 1, &rfds, NULL, NULL, NULL);
            if (s < 0) {
                ESP_LOGE(TAG, "select failed: errno %d", errno);
                break;
            } else {
                if (FD_ISSET(s_dns_server_socket_fd, &rfds)) {
                    /* DNS request data needs to be processed */
                    int len = recvfrom(s_dns_server_socket_fd, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

                    if (len < 0) {
                        ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                        break;
                    } else {
                        /* Get the sender's ip address as string */
                        if (source_addr.sin6_family == PF_INET) {
                            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
                        } else if (source_addr.sin6_family == PF_INET6) {
                            inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
                        }

                        rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
                        ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
#ifdef ESP_OPEN_DNS_REAUEST_DOMAIN_LOG // This is just for test
                        printf("DNS request is:");
                        for (int i = 0x4; i < len; i++) {
                            if ((rx_buffer[i] >= 'a' && rx_buffer[i] <= 'z') || (rx_buffer[i] >= 'A' && rx_buffer[i] <= 'Z') || (rx_buffer[i] >= '0' && rx_buffer[i] <= '9')) {
                                printf("%c", rx_buffer[i]);
                            } else {
                                printf("_");
                            }
                        }
                        printf("\r\n");
#endif
                        char reply[AT_WEB_DNS_MAX_LEN];
                        int reply_len = parse_dns_request(rx_buffer, len, reply, AT_WEB_DNS_MAX_LEN);

                        ESP_LOGD(TAG, "DNS reply with len: %d", reply_len);
                        if (reply_len <= 0) {
                            ESP_LOGD(TAG, "Failed to prepare a DNS reply");
                        } else {
                            int err = sendto(s_dns_server_socket_fd, reply, reply_len, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                            if (err < 0) {
                                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                                break;
                            }
                        }
                    }
                }

                /* Localhost receives the data indicating that ESP-AT wants to exit the dns_server_task */
                if (FD_ISSET(s_dns_server_localhost_fd, &rfds)) {
                    int len = recvfrom(s_dns_server_localhost_fd, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
                    if (len > 0) {
                        if (strncmp(rx_buffer, AT_WEB_TASK_EXIT_STR, strlen(AT_WEB_TASK_EXIT_STR)) == 0) {
                            goto dns_server_end;
                        }
                    }
                }
            }
        }

        web_dns_socket_close();
    }

dns_server_end:
    /* Unable to create socket, exit the task */
    web_dns_socket_close();
    xEventGroupSetBits(s_web_dns_event_group, AT_WEB_TASK_EXIT_SUCCESS_BIT);
    vTaskDelete(NULL);
}

void at_dns_server_start(void)
{
    if (s_web_dns_event_group == NULL) {
        s_web_dns_event_group = xEventGroupCreate();
    }

    xTaskCreate(dns_server_task, "dns", 3072, NULL, CAPTIVE_PORTAL_DNS_SERVER_TASK_PRIORITY, NULL);

    printf("dns server start\r\n");
}

void at_dns_server_stop(void)
{
    sendto(s_dns_server_localhost_fd, AT_WEB_TASK_EXIT_STR, strlen(AT_WEB_TASK_EXIT_STR), 0, (struct sockaddr *)&s_dest_addr, sizeof(s_dest_addr));

    EventBits_t bits = xEventGroupWaitBits(s_web_dns_event_group,
                                           AT_WEB_TASK_EXIT_SUCCESS_BIT,
                                           pdTRUE,
                                           pdFALSE,
                                           portMAX_DELAY);

    printf("dns server stop\r\n");
}
#endif
