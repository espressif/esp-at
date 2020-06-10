#ifndef GUIDE_BOOT_H_
#define GUIDE_BOOT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Download ESP8266 boot fireware to ESP8266-S2 module 
 *
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
esp_err_t esp_download_fw(void);

#ifdef __cplusplus
}
#endif

#endif /* GUIDE_BOOT_H_ */