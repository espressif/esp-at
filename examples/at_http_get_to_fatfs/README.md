### Usage
1. Add this `at_http_get_to_fatfs` component into the build system of esp-at project.

- Linux or macOS
```
export AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)
```

- Windows
```
set AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)
```

Notes:
- You need to replace (path_of_at_custom_cmd) with your real absolute path of your `at_custom_cmd` directory.
- You can specify multiple components. For example: `export AT_CUSTOM_COMPONENTS="~/prefix/my_path1 ~/prefix/my_path2"`.

2. Compile your firmware according to the [Compile ESP-AT Project](https://docs.espressif.com/projects/esp-at/en/latest/esp32/Compile_and_Develop/How_to_clone_project_and_compile_it.html#compile-esp-at-project-locally) guide.

3. Send a new AT command based on your situation.

The new command format is:
```
AT+HTTPGET_TO_FS=<"dst_path">,<url_len>
```
where:
- <"dst_path">: Specify the destination path where the file will be stored on the filesystem after the HTTP GET request is successfully executed.
- <url_len>: URL length. Maximum: 8192 bytes.

For example: url is "http://192.168.200.249:8080/a.bin", and you want to save it to `foo.bin`.
```
AT+HTTPGET_TO_FS="foo.bin",33
// then input http://192.168.200.249:8080/a.bin
```

4. Check the file content

Please send [AT+FS](https://docs.espressif.com/projects/esp-at/en/latest/esp32/AT_Command_Set/FS_AT_Commands.html#at-fs-filesystem-operations) command to read the data of your <"dst_path">.

For example: read 100 bytes from offset 0 of `foo.bin` file.
```
AT+FS=0,2,"foo.bin",0,100
```
