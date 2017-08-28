/* Authentication management (oauth/socket)
   August 2017
*/

#ifndef AUTHENTICATION_UTIL_H
#define AUTHENTICATION_UTIL_H

// System
#include <stdbool.h>

#define MAX_TOKEN_SIZE 		100
#define MAX_SSID_SIZE 		32
#define MAX_PASSWORD_SIZE	64

typedef struct
{
    char* token;
} token_struct;

typedef struct
{
    char* ssid;
    char* password;
} wifi_struct;

// Cred storage
bool write_wifi_creds_to_nv ( const char* ssid, const char* password );
wifi_struct* create_wifi_creds_from_nv ( void );
bool write_token_to_nv ( const char* token );
token_struct* create_token_from_nv ( void );
void free_token_struct(token_struct* token);
void free_wifi_struct(wifi_struct* wifi);

#endif // AUTHENTICATION_UTIL_H
