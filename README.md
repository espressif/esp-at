ESP-WolfSSL
===========

# Licensing

Espressif has a license to distribute wolfSSL royalty-free in binary form on all Espressif MCU products.
You can use the binary wolfSSL library on Espressif devices in products, without additional royalty costs.
The binary library does not include wolfSSL features for TLS 1.3, FIPS or DTLS 1.2.

Updates to these WolfSSL binary libraries will be available until at least March 2021.

To discuss additional features, additional support and updates, or to obtain a quote for other individual licensing options (commercial usage beyond March 2021) please contact licensing@wolfssl.com.

# Getting Started

- Please clone this repository using,
    ```
    git clone https://github.com/espressif/esp-wolfssl
    ```
- Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html for setting ESP-IDF
  - ESP-IDF can be downloaded from https://github.com/espressif/esp-idf/
  - ESP-IDF v4.1 and above is recommended version
- Please refer to [example README](examples/README.md) for more information on setting up examples

# Additional Pointers

In general, these are links which will be useful for using both wolfSSL, as well as networked and secure applications in general. Furthermore, there is a more comprehensive tutorial that can be found in Chapter 11 of the official wolfSSL manual. The examples in the wolfSSL package and Chapter 11 do appropriate error checking, which is worth taking a look at. For a more comprehensive API, check out chapter 17 of the official manual.

- WolfSSL Manual [https://www.wolfssl.com/docs/wolfssl-manual/]()
- WolfSSL GitHub
  [https://github.com/wolfssl/wolfssl]()
