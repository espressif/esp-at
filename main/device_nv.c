/* Simple NV implementation for housing 
   provisioning-related data
*/

// System
#include <string.h>
#include <netdb.h>
#include "stdbool.h"

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ESP-IDF
#include "esp_system.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "rom/crc.h"

static const char *TAG = "device_nv.c";

#warning "Maybe alow namespace switching in NV functions"
#define DEVICE_STORAGE_NAMESPACE "devicestorage"

esp_err_t nvs_init( void )
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_LOGW( TAG, "nvs_flash_init failed (0x%x), erasing partition and retrying", err );
        const esp_partition_t* nvs_partition = 0;
        esp_partition_find_first( ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL );
        assert(nvs_partition && "partition table must have an NVS partition");
        ESP_ERROR_CHECK( esp_partition_erase_range( nvs_partition, 0, nvs_partition->size) );
        err = nvs_flash_init();
    }
    return err;
}

esp_err_t nvs_read_string( char* attribute_key, char* data, size_t* length )
{
    nvs_handle my_handle = 0;
    esp_err_t err = ESP_OK;

    // OPEN
    ESP_LOGD( TAG, "Opening Non-Volatile Storage (NVS)... " );
    err = nvs_open(DEVICE_STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE( TAG, "Error (%d) opening NVS!", err);
    }
    else
    {
        ESP_LOGD( TAG, "Done");

        // READ
        ESP_LOGD( TAG, "Reading %s data from NVS... ", attribute_key );
        err = nvs_get_str( my_handle, attribute_key, data, length );
        #warning "need null termination here. check function, check length as well."
        switch (err)
        {
            case ESP_OK:
                ESP_LOGD( TAG, "Done");
                ESP_LOGD( TAG, "%s = %s", attribute_key, data );
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGD( TAG, "The value is not initialized yet, returning NULL");
                err = ESP_OK;
                data = "\0";
                break;
            default :
                ESP_LOGE( TAG, "Error (%d) reading!", err);
        }

        // CLOSE
        nvs_close( my_handle );
    }

    ESP_LOGD( TAG, "Finished, flush..");
    fflush( stdout );

    return err;
}


esp_err_t nvs_write_string( char* attribute_key, const char* data, bool verify )
{
    nvs_handle my_handle = 0;
    esp_err_t err = ESP_OK;

    // OPEN
    ESP_LOGI( TAG, "Opening Non-Volatile Storage (NVS) ... " );
    err = nvs_open( DEVICE_STORAGE_NAMESPACE, NVS_READWRITE, &my_handle );
    if (err != ESP_OK) {
        ESP_LOGE( TAG, "Error (%d) opening NVS!", err );
    } else {
        ESP_LOGD( TAG, "Done");

        // WRITE
        ESP_LOGI( TAG, "Writing data: %s to %s attribute from NVS... ", data, attribute_key );
        err = nvs_set_str(my_handle, attribute_key, data);
        if( err != ESP_OK )
        {
            ESP_LOGE( TAG, "Failed!" );
        }
        else
        {
            ESP_LOGD( TAG, "Done!" );
        }

        // COMMIT
        ESP_LOGI( TAG, "Committing updates in NVS... " );
        err = nvs_commit(my_handle);
        if( err != ESP_OK )
        {
            ESP_LOGE( TAG, "Failed!" );
        }
        else
        {
            ESP_LOGD( TAG, "Done!" );
        }

        // READ/VERIFY
        if( verify )
        {
        	#warning "Implement Read/Verify here."
        	ESP_LOGE( TAG, "Verify not implemented!" );
        }

        // CLOSE
        nvs_close(my_handle);

    }

    ESP_LOGI( TAG, "Finished, flush..");
    fflush(stdout);

    return err;
}