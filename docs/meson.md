### Getting Meson
https://mesonbuild.com/Getting-meson.html
### Meson build directory
Per compilare il progetto, Meson richiede una directory dedicata per archiviare tutti gli artefatti generati durante
il processo di build. Questa directory, chiamata build directory, conterrà tutti i file risultanti dalla compilazione.

Il vantaggio di utilizzare directory separate è che è possibile gestire più build directories contemporaneamente,
ciascuna con una configurazione diversa. Questo permette di testare vari settaggi o versioni del progetto senza dover
ricompilare tutto da zero ogni volta, migliorando così la flessibilità e l'efficienza del processo di sviluppo.

La build directory è locale e non va pushata sulla repository.

Per creare una build directory, eseguire il seguente comando dalla root del progetto:
```sh
meson setup buildDir
```
dove _buildDir_ è il nome della build directory. Il nome della build directory è arbitrario.

Equivalentemente:
```sh
meson buildDir
```
il comando di default se non specificato è `meson setup`.
Meson creerà una build directory chiamata _buildDir_ configurata in base ai files `meson.build`s e `meson_options.txt`.

Per compilare il progetto:
```sh
meson compile -C buildDir
```
Si noti che è necessario specificare il percorso della build directory.

Per eseguire i tests:
```sh
meson test -C buildDir
```

### Meson Options
In Meson esistono due tipi di options:
- [Built-in options](https://mesonbuild.com/Builtin-options.html)
- [Build options](https://mesonbuild.com/Build-options.html)

Le build options sono quelle che definiamo noi per il nostro progetto. Queste si trovano nel file `meson_options.txt`.
Attualmente sono state definite due opzioni:
```meson
option('enable_tests', type : 'boolean', value : true) # abilita/disabilita i tests 
option('generate_parser', type : 'boolean', value : true)  # genera il parser
```

Per modificare una opzione qualsiasi, eseguire questo comando:
```sh
meson configure -D${OPTION_NAME}=${OPTION_VALUE} ${BUILD_DIRECTORY_PATH} 
```

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