/* Authentication management (oauth/socket)
   August 2017
*/

// System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

// ESP-IDF
#include "esp_log.h"
#include "esp_system.h"

// Device
#include "authentication_util.h"
#include "system_util.h"
#include "device_nv.h"

static const char *TAG = "authentication_util.c";

void free_token_struct(token_struct* token)
{
    free(token->token);
    free(token);
}

void free_wifi_struct(wifi_struct* wifi)
{
    free(wifi->password);
    free(wifi);
}

token_struct* create_token_from_nv ( void )
{
    size_t size = MAX_TOKEN_SIZE-1;
    token_struct* token = malloc(sizeof(token_struct));
    char* data = malloc(MAX_TOKEN_SIZE);
    token->token = data;
    nvs_read_string( AUTH_TOKEN, token->token, &size );
    return token;
}

bool write_token_to_nv ( const char* token )
{
    nvs_write_string( AUTH_TOKEN, token, false );
    return true;
}

wifi_struct* create_wifi_creds_from_nv ( void )
{
    size_t size = 0; 
    wifi_struct* wifi = malloc(sizeof(wifi_struct));
    char* ssid = malloc(MAX_SSID_SIZE);
    char* password = malloc(MAX_PASSWORD_SIZE);
    wifi->ssid = ssid;
    wifi->password = password;

    size = MAX_SSID_SIZE;
    nvs_read_string( WIFI_SSID, wifi->ssid, &size );
    size = MAX_PASSWORD_SIZE;
    nvs_read_string( WIFI_PASSWORD, wifi->password, &size );
    return wifi;
}

bool write_wifi_creds_to_nv ( const char* ssid, const char* password )
{
    nvs_write_string( WIFI_SSID, ssid, false );
    nvs_write_string( WIFI_PASSWORD, password, false );
    return true;
}