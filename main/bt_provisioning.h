/* Start and manage BT server for purpose of provisioning device
   August 2017
   - Accept JSON string with auth/token detials
   - Process and move onward
*/

#ifndef BT_PROVISIONING_H
#define BT_PROVISIONING_H
#include <stdbool.h>

void bt_provisioning( void *pvParameter );
bool check_credentials( void );
bool clear_credentials( void );

#endif // BT_PROVISIONING_H
