
file(GLOB_RECURSE srcs *.c)
idf_component_register(
    SRCS "app_main.c" 
    INCLUDE_DIRS "." 
    REQUIRES 
        esp_adc         # ADC组件
        esp_mqtt       # MQTT组件
        nvs_flash      # NVS组件
        esp_at         # AT命令组件
)
set(includes "interface/include")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${includes})
