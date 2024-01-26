### Description
This `at_custom_cmd` example works as a component, allowing you to add your own AT commands under `at_custom_cmd` directory without modifying the [esp-at repository](https://github.com/espressif/esp-at) source code. This allows you to just host `at_custom_cmd` component in your git repository in a light-weight way, and it also allows you to freely choose any commit from the ESP-AT repository snapshot starting from February 2024 as the main project.

### Usage
1. Add your codes into `at_custom_cmd/custom/**/*.c` and `at_custom_cmd/custom/**/*.h` files (you can also create new source files and header files).
2. You may need to add more component dependencies into `at_custom_cmd/CMakeLists.txt` file if other components are used in your code.
3. Add your `at_custom_cmd` component into the build system of esp-at project.

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

4. Compile your firmware according to the [Compile ESP-AT Project](https://docs.espressif.com/projects/esp-at/en/latest/esp32/Compile_and_Develop/How_to_clone_project_and_compile_it.html#compile-esp-at-project-locally) guide.
