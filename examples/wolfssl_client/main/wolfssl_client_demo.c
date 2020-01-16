/* wolfSSL example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include <sys/socket.h>
#include <netdb.h>
#include "lwip/apps/sntp.h"
#include "protocol_examples_common.h"
#include <wolfssl/wolfcrypt/settings.h>
#include "wolfssl/ssl.h"

#if CONFIG_CERT_AUTH
extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");
#endif

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "www.howsmyssl.com"
#define WEB_PORT 443
#define WEB_URL "https://www.howsmyssl.com/a/check"

#define REQUEST "GET " WEB_URL " HTTP/1.0\r\n" \
    "Host: "WEB_SERVER"\r\n" \
    "User-Agent: esp-idf/1.0 espressif\r\n" \
    "\r\n"


//#define DEBUG_WOLFSSL_ON //Note : Uncomment this flag and add following line
//#define DEBUG_WOLFSSL
//to file components/wolfssl/port/user_settings.h
//to turn on Debug logs for wolfssl component

#define WOLFSSL_DEMO_THREAD_NAME        "wolfssl_client"
#ifdef DEBUG_WOLFSSL_ON
#define WOLFSSL_DEMO_THREAD_STACK_WORDS 4096
#else
#define WOLFSSL_DEMO_THREAD_STACK_WORDS 2048
#endif /* DEBUG_WOLFSSL_ON */
#define WOLFSSL_DEMO_THREAD_PRORIOTY    6
#define WOLFSSL_DEMO_SNTP_SERVERS       "time.google.com"

static const char *TAG = "wolfssl_client";

const char send_data[] = REQUEST;
const int32_t send_bytes = sizeof(send_data);
char recv_data[1024] = {0};

static void get_time()
{
    struct timeval now;
    int sntp_retry_cnt = 0;
    int sntp_retry_time = 0;

    sntp_setoperatingmode(0);
    sntp_setservername(0, WOLFSSL_DEMO_SNTP_SERVERS);
    sntp_init();

    while (1) {
        for (int32_t i = 0; (i < (SNTP_RECV_TIMEOUT / 100)) && now.tv_sec < 1525952900; i++) {
            vTaskDelay(100 / portTICK_RATE_MS);
            gettimeofday(&now, NULL);
        }

        if (now.tv_sec < 1525952900) {
            sntp_retry_time = SNTP_RECV_TIMEOUT << sntp_retry_cnt;

            if (SNTP_RECV_TIMEOUT << (sntp_retry_cnt + 1) < SNTP_RETRY_TIMEOUT_MAX) {
                sntp_retry_cnt ++;
            }

            ESP_LOGI(TAG,"SNTP get time failed, retry after %d ms", sntp_retry_time);
            vTaskDelay(sntp_retry_time / portTICK_RATE_MS);
        } else {
            ESP_LOGI(TAG,"SNTP get time success");
            break;
        }
    }
}

static void wolfssl_client(void* pv)
{
#ifdef DEBUG_WOLFSSL_ON
    wolfSSL_Debugging_ON();
#endif /* DEBUG_WOLFSSL_ON */
    int32_t ret = 0;

    const portTickType xDelay = 500 / portTICK_RATE_MS;
    WOLFSSL_CTX* ctx = NULL;
    WOLFSSL* ssl = NULL;

    int32_t sockfd = -1;
    struct sockaddr_in sock_addr;
    struct hostent* entry = NULL;

    /* CA date verification need system time */
    get_time();

    while (1) {

        ESP_LOGI(TAG,"Setting hostname for TLS session...");

        /*get addr info for hostname*/
        do {
            entry = gethostbyname(WEB_SERVER);
            vTaskDelay(xDelay);
        } while (entry == NULL);

        ESP_LOGI(TAG,"Init wolfSSL...");
        ret = wolfSSL_Init();

        if (ret != WOLFSSL_SUCCESS) {
            ESP_LOGI(TAG,"Init wolfSSL failed:%d...", ret);
            goto failed1;
        }

        ESP_LOGI(TAG,"Set wolfSSL ctx ...");
        ctx = wolfSSL_CTX_new(wolfTLSv1_2_client_method());

        if (!ctx) {
            ESP_LOGI(TAG,"Set wolfSSL ctx failed...");
            goto failed1;
        }

        ESP_LOGI(TAG,"Create socket ...");
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) {
            ESP_LOGI(TAG,"Create socket failed...");
            goto failed2;
        }

#if CONFIG_CERT_AUTH
        ESP_LOGI(TAG,"Loading the CA root certificate...");
        ret = wolfSSL_CTX_load_verify_buffer(ctx, server_root_cert_pem_start, server_root_cert_pem_end - server_root_cert_pem_start, WOLFSSL_FILETYPE_PEM);

        if (WOLFSSL_SUCCESS != ret) {
            ESP_LOGI(TAG,"Loading the CA root certificate failed...");
            goto failed3;
        }

        wolfSSL_CTX_set_verify(ctx, WOLFSSL_VERIFY_PEER, NULL);
#else
        wolfSSL_CTX_set_verify(ctx, WOLFSSL_VERIFY_NONE, NULL);
#endif

        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_port = htons(WEB_PORT);
        sock_addr.sin_addr.s_addr = ((struct in_addr*)(entry->h_addr))->s_addr;

        ESP_LOGI(TAG,"Connecting to %s:%d...", WEB_SERVER, WEB_PORT);
        ret = connect(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));

        if (ret) {
            ESP_LOGI(TAG,"Connecting to %s:%d failed: %d", WEB_SERVER, WEB_PORT, ret);
            goto failed3;
        }

        ESP_LOGI(TAG,"Create wolfSSL...");
        ssl = wolfSSL_new(ctx);

        if (!ssl) {
            ESP_LOGI(TAG,"Create wolfSSL failed...");
            goto failed3;
        }

        wolfSSL_set_fd(ssl, sockfd);

        ESP_LOGI(TAG,"Performing the SSL/TLS handshake...");
        ret = wolfSSL_connect(ssl);

        if (WOLFSSL_SUCCESS != ret) {
            ESP_LOGI(TAG,"Performing the SSL/TLS handshake failed:%d", ret);
            goto failed4;
        }

        ESP_LOGI(TAG,"Writing HTTPS request...");
        ret = wolfSSL_write(ssl, send_data, send_bytes);

        if (ret <= 0) {
            ESP_LOGI(TAG,"Writing HTTPS request failed:%d", ret);
            goto failed5;
        }

        ESP_LOGI(TAG,"Reading HTTPS response...");

        do {
            ret = wolfSSL_read(ssl, recv_data, sizeof(recv_data));


            if (ret <= 0) {
                ESP_LOGI(TAG,"Connection closed");
                break;
            }

            /* Print response directly to stdout as it is read */
            for (int i = 0; i < ret; i++) {
                printf("%c", recv_data[i]);
            }
	    printf("\n");
        } while (1);

failed5:
        wolfSSL_shutdown(ssl);
failed4:
        wolfSSL_free(ssl);
failed3:
        close(sockfd);
failed2:
        wolfSSL_CTX_free(ctx);
failed1:
        wolfSSL_Cleanup();

        for (int countdown = 10; countdown >= 0; countdown--) {
            ESP_LOGI(TAG,"%d...", countdown);
            vTaskDelay(1000 / portTICK_RATE_MS);
        }

        ESP_LOGI(TAG,"Starting again!");
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
    xTaskCreate(wolfssl_client,
                WOLFSSL_DEMO_THREAD_NAME,
                WOLFSSL_DEMO_THREAD_STACK_WORDS,
                NULL,
                WOLFSSL_DEMO_THREAD_PRORIOTY,
                NULL);
}
