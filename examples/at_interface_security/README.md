# Introduction
This `at_interface_security` example demonstrates how to securely communicate between the ESP device and the host MCU over AT commands. Traditionally, AT commands are transmitted in plaintext, but in this implementation, both the ESP device and the host MCU support encryption and decryption of data to ensure secure communication.

**Features:**  
- Secure communication between the device and the host MCU.
- `AES-CTR` encryption/decryption of AT command exchanges.
- Python script (`at_intf_security_host.py`) simulates host MCU for testing purposes.

# Usage
1. Add this `at_interface_security` component into the build system of esp-at project.

- Linux or macOS
```
export AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)
```

- Windows
```
set AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)
```

**Notes:**
- You need to replace (path_of_at_custom_cmd) with your real absolute path of your `at_interface_security` directory.

2. Build the project and flash the firmware to the module according to the [Compile ESP-AT Project](https://docs.espressif.com/projects/esp-at/en/latest/esp32/Compile_and_Develop/How_to_clone_project_and_compile_it.html#compile-esp-at-project-locally) guide.

3. Configure and run `python at_intf_security_host.py` script on your PC.

**Notes:**
- You may need to modify some configurations. Please refer to the comments at the beginning of the `at_intf_security_host.py` script.

4. See the script outputs.

If everything goes well, the output should be as is:
```
...
Waiting for AT ready... (You may need to reset or power on the ESP module)
...
<reset the ESP module manually>
...
<ESP module power-on logs>
...
intf-sec-rx: a1 61 07 bf 80 39 c9 54 13
// This is the binary stream received from the ESP module. This should be "\r\nready\r\n" (9 bytes) after decryption.

...
intf-sec-tx: ed 3f 78 d0
// This is the binary stream that is ready to be sent to the ESP module. This should be "AT\r\n" (4 bytes) after decryption.

...
intf-sec-rx: 74 bd 1b 0f c9 76 69 38 eb af
// This is the binary stream received from the ESP module. This should be "AT\r\n\r\nOK\r\n" (10 bytes) after decryption.

...
<more interaction logs>
...
AT interface security test success!
at_cmd_port_thread exited.
at_log_port_thread exited.

All done! You can check the log:

  <path_of_log>

```

# Security Considerations
### AES Key and IV Management
In this example, the default AES key is a 16-byte string of 'A' (b'AAAAAAAAAAAAAAAA'), and the AES IV is a 16-byte string of 'T' (b'TTTTTTTTTTTTTTTT'). It is crucial to change these values to something secure and and avoid storing them in plain text. We strongly recommend dynamically generating the AES key and IV at runtime to prevent reverse engineering attacks. To generate a secure AES key and IV, consider using the following function:

```
(key, IV)=F(x, y, z)
```

Where:
- `x, y, z` are input parameters derived from various sources, such as the device's MAC address, eFuse status, or your own data. These inputs add entropy and enhance the unpredictability of the generated key and IV.
- `F` is a carefully designed function that ensures secure output, which could involve mathematical operations, hash functions, or your own combination of functions.

This method increases the security of the AES key and IV by making them highly unpredictable.

### Alternative Encryption Algorithms
If the host MCU does not support AES-CTR encryption, you can choose alternative stream ciphers like `ChaCha20` or `Salsa20`. These algorithms are efficient and well-suited for resource-constrained devices. Block cipher algorithms, such as AES in CBC or ECB mode, are not recommended due to potential vulnerabilities in certain contexts.

### Security with mbedtls_cxx
If you require even more secure AT command interactions, you can consider using the [mbedtls_cxx](https://components.espressif.com/components/espressif/mbedtls_cxx) library to implement the wrapper of mbedTLS for performing interface security. This approach will provide additional layers of encryption and authentication. However, this requires the MCU to support the mbedtls library (TLS 1.2 or higher) and will involve additional development time for integration.
