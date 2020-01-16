ESP-WOLFSSL
===========

# Licensing

Espressif has a license to distribute wolfSSL(v4.3) royalty-free in binary form on all Espressif MCU products.
You can use the binary wolfSSL library on Espressif devices in products, without additional royalty costs.
The binary library does not include wolfSSL features for TLS 1.3, FIPS or DTLS 1.2.

Updates to these wolfSSL binary libraries will be available until at least March 2021.

To discuss additional features, additional support and updates, or to obtain a quote for other individual licensing options (commercial usage beyond March 2021) please contact licensing@wolfssl.com.

# Requirements
- ESP_IDF
 - To run the examples user must have installed ESP-IDF version v4.1 (minimum supported) from https://github.com/espressif/esp-idf.git
 - The IDF_PATH should be set as an environment variable

# Getting Started

- Please clone this repository using,
 ```
 git clone https://github.com/espressif/esp-wolfssl
 ```
- Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html for setting ESP-IDF
 - ESP-IDF can be downloaded from https://github.com/espressif/esp-idf/
 - ESP-IDF v4.1 and above is recommended version
- Please refer to [example README](examples/README.md) for more information on setting up examples

# Options (Debugging and more)
- `esp-wolfssl` esp-tls related options can be obtained by choosing SSL library as `wolfSSL` in `idf.py/make menuconfig -> Component Config -> ESP-TLS -> choose SSL Library `.
It shows following options

    - Enable SMALL_CERT_VERIFY
        - This is a flag used in wolfSSL component and is enabled by default in `esp-wolfssl`.
        - Enabling this flag allows user to authenticate the server by providing the Intermediate CA certificate of the server, for a more strict check disable this flag after which you will have to provide the root certificate at top of the hierarchy of certificate chain which will have `Common Name = Issuer Name`, Such a strict check is not compulsary in most cases hence by default the flag is enabled but the option is provided for the user.

    - Enable Debug Logs for wolfSSL
        - This option prints detailed logs of all the internal operations, highly useful when debugging an error.

- `esp-wolfssl` specific options (see NOTE) are available under `idf.py/make menuconfig -> Component Config -> wolfSSL`.

    - Enable ALPN ( Application Layer Protocol Negotiation ) in wolfSSL
        - This option is enabled by default for wolfSSL, and can be disabled if not required.
---
**NOTE**
 These options are valid for `esp-tls` only if `wolfSSL` is selected as its SSL/TLS Library.
---
# Comparison of wolfSSL and mbedTLS

The following table shows a typical comparison between wolfSSL and mbedtls when `https_request` (which has server authentication) was run with both
SSL/TLS libraries and with all respective configurations set to default.
_(mbedtls IN_CONTENT length and OUT_CONTENT length were set to 16384 bytes and 4096 bytes respectively)_

| Property | wolfSSL | mbedTLS |
|--------------------|----------|----------|
| Total Heap Consumed| ~19 Kb | ~37 Kb |
| Task Stack Used | ~2.2 Kb | ~3.6 Kb |
| Bin size | ~858 Kb | ~736 Kb |

# Additional Pointers

In general, these are links which will be useful for using both wolfSSL, as well as networked and secure applications in general. Furthermore, there is a more comprehensive tutorial that can be found in Chapter 11 of the official wolfSSL manual. The examples in the wolfSSL package and Chapter 11 do appropriate error checking, which is worth taking a look at. For a more comprehensive API, check out chapter 17 of the official manual.

- wolfSSL Manual [https://www.wolfssl.com/docs/wolfssl-manual/]()
- wolfSSL GitHub
 [https://github.com/wolfssl/wolfssl]()

