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
Si noti che è necessario specificare il percorso della build directory. L'opzione `-C` sta per _change directory_. Prima
di eseguire il comando, Meson entra nella directory specificata.

Per eseguire i tests:
```sh
meson test -C buildDir
```
Alcuni test cases necessitano di un file di input per funzionare (ad esempio per leggere la specifica di un PolyhedralSystem,
eseguire il parsing e confrontare il risultato con l'output desiderato). Prima di eseguire i tests, è necessario rendere
disponibile questi file di input ai test eseguibili. Per farlo, eseguire il seguente comando:
```
meson install --tags tests
```
Il comando installerà i file di input nella build directory, nella stessa directory dei test eseguibili.
Cercherò di trovare un modo per evitare questo passaggio.

### Dipendenze
Il progetto ha bisogno delle seguenti dipendenze:
- PPL >= 14.0.0
- GMP >= 6.3.0
- GMPXX >= 6.3.0
- ANTLR4 Runtime >= 4.13.0: https://www.antlr.org/
- Catch2 >=3.7.1 (test framework)

Un ulteriore dipendenza è [ANTLR4 CLI Tools](https://github.com/antlr/antlr4-tools/blob/master/README.md). In particolare, deve essere possibile usare il comando `antlr4` per generare
automaticamente il parser durante la build.

Meson provvederà a installare automaticamente Catch2 come [subproject](https://mesonbuild.com/Subprojects.html).
Il resto delle dipendenze devono essere installate manualmente. Un modo per farlo è installare le dipendenze manualmente e successivamente
creare dei pkg config files per ogni dipendenza in uno dei percorsi specificati dal seguente comando:
```
pkg-config --variable pc_path pkg-config
```
Meson cercherà in uno di questo percorsi il pkg config file corrispondente a ogni dipendenza. Un pkg config file specifica
dove si trova la dipendenza sul sistema.
#### Esempio pkg config file
Su un sistema Ubuntu, tipicamente uno dei percorsi in cui vengono posizionati i pkg config files è `/usr/local/lib/pkgconfig`.
Per specificare, ad esempio, dove si trova la dipendenza ANTLR4 Runtime sul sistema, creiamo in questa directory un file
con lo stesso nome della dipendenza `antlr4-runtime.pc`. Il nome deve essere uguale a quello specificato in `src/meson.build`
dalla funzione `dependency('antlr4-runtime', version: '>=4.13.0')`:
```text
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

### Visualizzare la configurazione corrente di una build directory
Per visualizzare la configurazione corrente di una build directory (i valori concreti delle [Meson Options](#meson-options)) eseguire questo comando:
```
meson configure <BUILD_DIRECTORY_PATH>
```
Per visualizzare la configurazione di default determinata dai meson files, eseguire dalla root del progetto:
```
meson configure
```
Questi comandi mostrano tutte le possibili opzioni/configurazioni, i loro valori attuali e i loro possibili valori.

### Riflettere le nuove configurazioni in una build directory già esistente
Quando si aggiungono nuove opzioni nel file `meson_options.txt` oppure quando si modifica uno o più `meson.build` files 
oppure quando si aggiunge un nuovo `meson.build` file, per riflettere le nuove configurazioni in una build directory
già esistente, eseguire il seguente comando:
```
meson setup <BUILD_DIR_PATH> --reconfigure
```
equivalentemente:
```
meson <BUILD_DIR_PATH> --reconfigure
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
I possibili valori di `-Db_sanitize` sono:
- none
- address
- thread
- undefined
- memory
- address,undefined

### Eseguire i test con valgrind attivo
È necessario installare valgrind.
```shell
meson test --wrap='valgrind --leak-check=full --error-exitcode=1' testname
```
**ATTENZIONE:** per usare valgrind è necessario prima disabilitare ogni sanitizers usando questo comando:
```
meson configure -Db_sanitize=none buildDir
```
- https://mesonbuild.com/Unit-tests.html#other-test-options
- https://stackoverflow.com/questions/57734973/how-to-use-valgrind-with-meson