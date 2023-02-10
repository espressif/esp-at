### Description
To improve available heap size, `relink` can move some code from `.iram0.text` section to `.flash.text` section.

### Usage
1. Open the `build/esp-at.map` file, and find the function you want to move from `.iram0.text` section to `.flash.text` section.
2. Modify the `tools/relink/function.csv` file, and add a new line (library name, object name, function name, optional sdkconfig dependence) according to `build/esp-at.map` file.
3. Modify the `tools/relink/library.csv` file, and add a new line (library name, library path) according to `build/esp-at.map` file.
4. Modify the `tools/relink/object.csv` file, and add a new line (library name, object name, object path) according to `build/esp-at.map` file.
5. Delete `build` directory and re-compile the esp-at project.

### How it works
In short, in the link phase of the linker, this `relink` tool parses the `relink` directory, dumps the symbol tables (riscv32-esp-elf-objdump -t \<object>/\<library>), finds out all functions that need to be relinked, and then generate a new `esp-idf/esp_system/ld/sections.ld` according to the section information of the functions that need to be relinked. Then, the linker uses this new linker script `esp-idf/esp_system/ld/sections.ld` instead. So, these functions can be moved from `.iram0.text` section to `.flash.text` section.
