ESP-WolfSSL
===========

# Licensing

Espressif has a license to distribute wolfSSL royalty-free in binary form on all Espressif MCU products.
You can use the binary wolfSSL library on Espressif devices in products, without additional royalty costs.
The binary library does not include wolfSSL features for TLS 1.3, FIPS or DTLS 1.2.

Updates to these WolfSSL binary libraries will be available until at least March 2021.

To discuss additional features, additional support and updates, or to obtain a quote for other individual licensing options (commercial usage beyond March 2021) please contact licensing@wolfssl.com.

# Getting Started

- Requirements
	- ESP-IDF
	- ESP-WolfSSL

## How to use with ESP-IDF

If you want to use WolfSSL in your project, there are two ways in which you can proceed:

### Use `esp-wolfssl` component outside from ESP-IDF

- If you are using CMake as build system, add following line to the CMakeLists.txt file in your project folder

	`set(EXTRA_COMPONENT_DIRS PATH-to-your-esp-wolfssl-folder)`

	see the guidelines for `EXTRA_COMPONENT_DIRS` at

	[https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#optional-project-variables]()
- If you are using GNU Make as build system, add following lines to the Makefile file in your project folder
	`EXTRA_COMPONENT_DIRS:= PATH-to-your-esp-wolfssl-folder`

	see the guidelines at

    [https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system-legacy.html#optional-project-variables]()

### Include `esp-wolfssl` component in ESP-IDF

- Copy folder `esp-wolfssl` into `$(IDF_PATH)/components/`

## Selecting WolfSSL

- To select the esp-wolfssl as SSL/TLS Library in `esp-tls`
	`idf.py/make menuconfig -> Component Config -> ESP-TLS -> Choose ESP_TLS/SSL Library`
	- note: esp-wolfssl is used with ESP-TLS abstraction, your code written using esp-tls component should run successfully with choosing wolfssl

- You can see the setting of `esp-wolfssl` at `esp-wolfssl/wolfssl/port/user_settings`. Additionally enable debug logs for wolfssl in the same option provided in step 3 (helps while debugging).

# Additional Pointers

In general, these are links which will be useful for using both wolfSSL, as well as networked and secure applications in general. Furthermore, there is a more comprehensive tutorial that can be found in Chapter 11 of the official wolfSSL manual. The examples in the wolfSSL package and Chapter 11 do appropriate error checking, which is worth taking a look at. For a more comprehensive API, check out chapter 17 of the official manual.

- WolfSSL Manual [https://www.wolfssl.com/docs/wolfssl-manual/]()
- WolfSSL GitHub
  [https://github.com/wolfssl/wolfssl]()
