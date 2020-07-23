如何更新 esp-idf 版本
========================

当前工程默认支持 ESP32 UART AT 和 ESP8266 UART AT 平台，每个平台对应一套配置文件，配置文件目录可以通过 Makefile 文件中的 `ESP_AT_MODULE_CONFIG_DIR` 变量指定，默认 ESP32 UART AT 配置目录为
```module_config/module_esp32_default```，ESP8266 UART AT 配置文件目录为 ```module_config/module_esp8266_default```，具体的版本信息在配置目录下的 IDF_VERSION 文件中，如 ESP32 平台的配置信息为：

```
branch:master
commit:7fa98593bc179ea50a1bc8244d5b94bac59c9a10
repository:https://github.com/espressif/esp-idf.git
```

第一行的 branch 代表当前所使用 idf 的分支名称  
第二行的 commit 代表当前所使用 idf 的 commit id  
第三行的 repository 代表当前所使用 idf 的仓库 url  

如果想要更新时，只需要将上面的 branch、commit 和 repository 修改为自己想要的分支即可。

***当更新 esp-idf 版本信息后，建议将当前工程下的 esp-idf 目录删除，下一次编译时，会重新 clone 新的 esp-idf***  

**注意：如果 repository url 发生了变化，必须将当前工程下的 esp-idf 目录删除，否则会更新失败。**
