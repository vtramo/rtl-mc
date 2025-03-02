### Come eseguire i tests
- `meson test -C buildDir system-parser -v` (PolyhedralSystem parser)
- `meson test -C buildDir system-observables -v` (PolyhedralSystem generate obersevables)
- `meson test -C buildDir tile -v` (Tile extractor)
- `meson test -C buildDir rtlf-parser -v` (RTLf parser)
- `meson test -C buildDir discretisation -v` (discretisation)
- `meson test -C buildDir automaton -v` (automaton)
- `meson test -C buildDir spot-utils -v` (spot utils)
- `meson test -C buildDir denot-on-the-fly -v` (denot on the fly)
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