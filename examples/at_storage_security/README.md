# Introduction
This `at_storage_security` example demonstrates how to implement secure storage for sensitive data in the ESP-AT firmware. It automatically encrypts sensitive data before storing it in NVS (Non-Volatile Storage) and decrypts it when reading, ensuring that critical information such as Wi-Fi credentials and keys are protected from unauthorized access.

**Features:**
- Automatic encryption/decryption of sensitive NVS data using AES-CTR algorithm.
- Transparent operation - no changes needed to existing AT commands.
- Custom AT command (`AT+STORAGE_NVS_KEY`) to manage encryption keys.
- Selective encryption based on key names (e.g., keys containing "key", Wi-Fi passwords).

# How It Works
The example uses function wrapping to intercept NVS read/write operations:
- `__wrap_nvs_set_blob/str` - Encrypts sensitive data before writing to NVS.
- `__wrap_nvs_get_blob/str` - Decrypts sensitive data after reading from NVS.

The encryption process:
1. Generates a random IV (Initialization Vector) for each write operation.
2. Encrypts the data using AES-CTR with the configured key.
3. Stores IV + ciphertext together in NVS.
4. On read, extracts the IV and decrypts the ciphertext.

# Encrypted Data
By default, the following data is automatically encrypted:
- NVS keys containing the word "key"
- Wi-Fi station password (`sta.pswd`)
- Wi-Fi AP information (`sta.apinfo`)

You can customize which keys are encrypted by modifying the `at_nvs_key_encrypt()` function in `custom/at_custom_cmd.c`.

# Usage

## 1. Add Component to Build System

- Linux or macOS
```bash
export AT_CUSTOM_COMPONENTS=(path_of_at_storage_security)
```

- Windows
```cmd
set AT_CUSTOM_COMPONENTS=(path_of_at_storage_security)
```

**Notes:**
- Replace `(path_of_at_storage_security)` with the real absolute path of your `at_storage_security` directory.
- You can specify multiple components. For example: `export AT_CUSTOM_COMPONENTS="~/prefix/my_path1 ~/prefix/my_path2"`.

## 2. Build and Flash

Build the project and flash the firmware to the module according to the [Compile ESP-AT Project](https://docs.espressif.com/projects/esp-at/en/latest/esp32/Compile_and_Develop/How_to_clone_project_and_compile_it.html#compile-esp-at-project-locally) guide.

## 3. Manage Encryption Key

### Query Current Key
```
AT+STORAGE_NVS_KEY?
```

Response:
```
++STORAGE_NVS_KEY:4553502d41542d4145532d4b45590000
OK
```
This returns the current AES key in hexadecimal format (32 hex characters = 16 bytes / 128 bits).

### Set New Key
```
AT+STORAGE_NVS_KEY="0123456789abcdef0123456789abcdef"
```

**Notes:**
- The key must be exactly 32 hexadecimal characters (16 bytes / 128 bits).
- After setting a new key, all subsequently stored data will be encrypted with the new key.
- Previously stored data encrypted with the old key will become inaccessible unless you switch back to the old key.
- **Important**: Store your encryption key securely! If the key is lost, encrypted data cannot be recovered.

## 4. Test Encryption

You can test the encryption by storing and retrieving Wi-Fi credentials:

```
AT+CWJAP="ssid","password"
```

The password "password" will be automatically encrypted before storing in NVS. To verify encryption is working, you can check the log output (requires setting log level to INFO):
```
I (xxxxx) at-storage-sec: nvs_set_blob encrypted, key:sta.pswd, len:65, ret:0x0
I (xxxxx) plaintext: <addr>   70 61 73 73 77 6f 72 64  00 00 00 00 00 00 00 00  |password........|
I (xxxxx) iv: ...
I (xxxxx) ciphertext: ...
```

## 5. Test Decryption

After the Wi-Fi credentials are stored with encryption in step 4, you can verify that the decryption works correctly by restarting the device:

```
AT+RST
```

After the device restarts, it will automatically read the encrypted Wi-Fi password from NVS, decrypt it, and reconnect to the AP. You should see the device successfully connect to the Wi-Fi network. To verify the decryption is working, check the log output:
```
I (xxxxx) at-storage-sec: nvs_get_blob decrypted, key:sta.pswd, len:65, ret:0x0
I (xxxxx) ciphertext: ...
I (xxxxx) iv: ...
I (xxxxx) plaintext: <addr>   70 61 73 73 77 6f 72 64  00 00 00 00 00 00 00 00  |password........|
```

This confirms that the encrypted password was successfully decrypted and used for Wi-Fi connection.

# Security Considerations

## AES Key Management
Instead of using the `AT+STORAGE_NVS_KEY` command to set the encryption key, you can implement dynamic key generation using device-specific data. For secure key generation methods, please refer to [AES Key and IV Management](https://github.com/espressif/esp-at/blob/master/examples/at_interface_security/README.md#aes-key-and-iv-management) in the at_interface_security example.

## Enhanced Storage Security
For even more secure storage, consider using ESP-IDF's built-in [NVS Encryption](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_encryption.html) feature to encrypt the entire NVS partition.

