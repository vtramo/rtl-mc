### Come eseguire i tests
- `meson test -C buildDir system-parser -v` (PolyhedralSystem parser)
- `meson test -C buildDir rtlf-parser -v` (RTLf parser)
- `meson test -C buildDir discretization -v` (discretization)
- `meson test -C buildDir automaton -v` (automaton)
- `meson test -C buildDir spot-utils -v` (spot utils)
- `meson test -C buildDir denot -v` (denot)
- `meson test -C buildDir -v` (tutti i test escluso i benchmarks)

### Eseguire i test con valgrind attivo
È necessario installare valgrind. Successivamente, eseguire i test (o una determinata suite di test) in questo modo:
```shell
meson test --wrap='valgrind --leak-check=full --error-exitcode=1' -C ${BUILD_DIR_PATH}
```
**ATTENZIONE:** per usare valgrind è necessario prima disabilitare ogni sanitizers usando questo comando:
```
meson configure -Db_sanitize=none buildDir
```
- https://mesonbuild.com/Unit-tests.html#other-test-options
- https://stackoverflow.com/questions/57734973/how-to-use-valgrind-with-meson


### Come risolvere l'errore `undefined reference to 'main'` (Catch2)
Quando si eseguono i test, raramente potrebbe apparire un errore del genere relativo a Catch2.

Per risolvere, eseguire i seguenti comandi:
```shell
meson compile -C buildDir --clean # pulisci build directory
meson subprojects purge catch2 --confirm # Elimina il subproject catch2
meson compile -C buildDir # ricompila l'intero progetto
```