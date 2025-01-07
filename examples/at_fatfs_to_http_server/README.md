### Usage
1. Add this `at_fatfs_to_http_server` component into the build system of esp-at project.

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
AT+FS_TO_HTTP_SERVER=<"dst_path">,<url_len>
```
where:
- <"dst_path">: Specify the destination path where the file will be posted from the filesystem after the HTTP POST request is successfully executed.
- <url_len>: URL length. Maximum: 8192 bytes.

For example: url is "http://httpbin.org/post", and you want to post file `foo.bin`.
```
AT+FS_TO_HTTP_SERVER="foo.bin",23
// then input http://httpbin.org/post
```
