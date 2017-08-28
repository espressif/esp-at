/* Simple NV implementation
   August 2017
   - For storing and retrieving provisioning-related data
*/

#ifndef DEVICE_NV_H
#define DEVICE_NV_H

#define TOKEN_KEY 		"token"
// Oauth
#define AUTH_KEY		"auth"
#define REFRESH_KEY		"refresh"
// Socket
#define ACCESS_TOKEN    "access_token"
#define IP				"ip"
#define READY			"ready"
// BT Provisioning
#define BT_PROV_READY	"btcreds_set"
#define AUTH_TOKEN		"auth_token"
#define WIFI_SSID		"wifi_ssid"
#define WIFI_PASSWORD 	"wifi_password"

esp_err_t nvs_read_string( char* attribute_key, char* data, size_t* length );
esp_err_t nvs_write_string( char* attribute_key, const char* data, bool verify );
esp_err_t nvs_init( void );

#endif // DEVICE_NV_H

