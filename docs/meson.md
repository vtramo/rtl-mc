### Getting Meson
See: https://mesonbuild.com/Getting-meson.html

### Meson Build Directory

To compile the project, Meson requires a dedicated directory to store all build artifacts. This directory, called the *build directory*, will contain all files generated during the compilation process.

The main advantage of using separate build directories is that you can manage multiple builds simultaneously, each with different configurations. This allows testing various settings or project versions without needing to rebuild everything from scratch, increasing flexibility and efficiency in the development process.

The build directory is local and **should not** be pushed to the repository.

To create a build directory, run the following command from the project root:
```sh
meson setup buildDir
```
Here, `buildDir` is the name of the build directory (you can choose any name). Meson will configure a build environment using the `meson.build` and `meson_options.txt` files.

To compile the project:
```sh
meson compile -C buildDir
```
Note: the `-C` flag means *change directory* — Meson will enter the specified directory before compiling.

To run the tests:
```sh
meson test -C buildDir
```

---

### Dependencies

This project depends on the following libraries:
- PPL >= 14.0.0
- GMP >= 6.3.0
- GMPXX >= 6.3.0
- ANTLR4 Runtime >= 4.13.0 → https://www.antlr.org/
- Catch2 >= 3.7.1 (test framework)
- SPOT 2.12.1

An additional requirement is the [ANTLR4 CLI Tools](https://github.com/antlr/antlr4-tools/blob/master/README.md). In particular, the `antlr4` command must be available to automatically generate the parser during the build process.

Meson will automatically install Catch2 as a [subproject](https://mesonbuild.com/Subprojects.html).

All other dependencies must be installed manually. One way to do this is to install them system-wide and provide `pkg-config` files for each dependency in one of the paths listed by:
```bash
pkg-config --variable pc_path pkg-config
```

Meson will search these paths for `.pc` files corresponding to the dependencies. Each `.pc` file describes where the dependency is located on the system.

#### Example `pkg-config` file

On Ubuntu systems, a common path for `.pc` files is `/usr/local/lib/pkgconfig`.

To make ANTLR4 Runtime available, create a file named `antlr4-runtime.pc` in that directory. The name must match what is referenced in `src/meson.build` like so:
```meson
dependency('antlr4-runtime', version: '>=4.13.0')
```

Example `antlr4-runtime.pc`:
```
prefix=/usr/local
includedir=${prefix}/include
exec_prefix=${prefix}
libdir=${exec_prefix}/lib

Name: AntLR V4 Runtime
Version: 4.13.2
Description:
URL:
Cflags: -I${includedir}
Libs: -L${libdir} -lantlr4-runtime
```

When installing libraries, `make install` often generates this file automatically. However, this is not always the case (e.g., ANTLR4 and PPL do not generate it, but SPOT does).

---

### Meson Options

Meson supports two kinds of options:
- [Built-in options](https://mesonbuild.com/Builtin-options.html)
- [Build options](https://mesonbuild.com/Build-options.html)

Build options are user-defined and declared in the `meson_options.txt` file.

Currently, three options are defined:
```meson
option('enable_tests', type : 'boolean', value : true) # enable/disable unit tests
option('generate_parser', type : 'boolean', value : true) # auto-generate parser
option('debug', type: 'boolean', value: true, description: 'Enable debug mode') # disables assertions and logging if false
```

To configure a specific option:
```sh
meson configure -D${OPTION_NAME}=${OPTION_VALUE} ${BUILD_DIRECTORY_PATH}
```

---

### View Current Build Directory Configuration

To inspect the current configuration of a build directory:
```sh
meson configure <BUILD_DIRECTORY_PATH>
```

To see the default configuration values defined by the Meson files:
```sh
meson configure
```

These commands list all options, their current values, and available settings.

---

### Reflect Changes in an Existing Build Directory

When new options are added to `meson_options.txt`, or when you modify/add `meson.build` files, you can reflect the changes using:
```sh
meson setup <BUILD_DIRECTORY_PATH> --reconfigure
```

Or start from scratch:
```sh
meson setup <BUILD_DIRECTORY_PATH> --wipe
```

---

### Sanitizers

You can enable built-in AddressSanitizer and UndefinedBehaviorSanitizer via:
```sh
-Db_sanitize=address,undefined
```

Reference commit: [bd3e75d](https://github.com/vtramo/rtl-mc/commit/bd3e75d5985c8429ce341ce27aed495e53645362)

- Meson Built-in Options: https://mesonbuild.com/Builtin-options.html
- Sanitizers documentation: https://github.com/google/sanitizers

To disable sanitizers:
```sh
meson configure -Db_sanitize=none <BUILD_DIRECTORY_PATH>
```

Possible values for `-Db_sanitize`:
- `none` (default)
- `address`
- `thread`
- `undefined`
- `memory`
- Multiple (e.g. `address,undefined`)