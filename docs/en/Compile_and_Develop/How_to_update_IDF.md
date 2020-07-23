How to update the esp-idf version
=================================

The esp-at project supports two platforms by default, ESP32 UART AT and ESP8266 UART AT. Each of the platform has a set of configurations, you can configure its directory by setting the `ESP_AT_MODULE_CONFIG_DIR` in the `Makefile`. The default directory of ESP32 UART AT is 
```module_config/module_esp32_default```, and the ESP8266 UART AT is  ```module_config/module_esp8266_default```, version information is in the file `IDF_VERSION`. For example, the version info of `module_esp32_default` is:

```
branch:master
commit:7fa98593bc179ea50a1bc8244d5b94bac59c9a10
repository:https://github.com/espressif/esp-idf.git
```

branch: branch of the idf  
commit: commit id of the idf  
repository: url of the idf  
 
To update the idf/SDK, you need to change the branch, commit and repository  to be the ones that you want to update to.


***It is suggested that you can delete the original idf/SDK after updating the file `IDF_VERSION`. In this case, the esp-at will clone the new idf/SDK according to the file `IDF_VERSION` firstly in next compilation.***  

**Notice: If you changed the repository url in the file `IDF_VERSION`, then you have to delete the original idf/SDK in the project. Otherwise, the update may fail.**
