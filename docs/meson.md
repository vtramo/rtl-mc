### Cambiare settings dopo che la build directory è stata generata
[Configuring a build directory](https://mesonbuild.com/Configuring-a-build-directory.html)
```sh
meson configure buildDir
meson compile -C buildDir
```
Per visualizzare la configurazione attuale, eseguire questo comando dalla root del progetto:
```sh
meson configure
```

### Sanitizers
Add Address and Undefined Behaviour sanitizers as built-in base options in the Meson build configuration via b_sanitize=address,undefined.

Commit SHA: [bd3e75d5985c8429ce341ce27aed495e53645362](https://github.com/vtramo/rtl-mc/commit/bd3e75d5985c8429ce341ce27aed495e53645362)

- Meson Builtin-options: https://mesonbuild.com/Builtin-options.html
- Sanitizers: https://github.com/google/sanitizers

Per disabilitare i sanitizers:
```shell
meson configure -Db_sanitize=none <BUILD_DIRECTORY_PATH>
```

### Eseguire i test con valgrind attivo
```shell
meson test --wrap='valgrind --leak-check=full --error-exitcode=1' testname
```
- https://mesonbuild.com/Unit-tests.html#other-test-options
- https://stackoverflow.com/questions/57734973/how-to-use-valgrind-with-meson