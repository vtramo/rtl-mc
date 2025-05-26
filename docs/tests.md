### How to Run Tests

- `meson test -C buildDir system-parser -v` (PolyhedralSystem parser)
- `meson test -C buildDir system-observables -v` (PolyhedralSystem generate observables)
- `meson test -C buildDir tile -v` (Tile extractor)
- `meson test -C buildDir rtlf-parser -v` (RTLf parser)
- `meson test -C buildDir discretisation -v` (Discretisation)
- `meson test -C buildDir automaton -v` (Automaton)
- `meson test -C buildDir spot-utils -v` (SPOT utilities)
- `meson test -C buildDir denot-on-the-fly -v` (On-the-fly denotation)
- `meson test -C buildDir -v` (Run all tests except benchmarks)

---

### List All Available Tests

```sh
meson test -C buildDir --list
```

Example output:
```
rtl-mc / automaton-on-the-fly
rtl-mc / formula-denotation-map
rtl-mc / discretisation
rtl-mc / system-parser
rtl-mc / rtl-parser
rtl-mc / ppl-utils
rtl-mc / remove-sing
rtl-mc / is-non-recurrent
rtl-mc / interior
rtl-mc / omnidirectional
rtl-mc / closed
rtl-mc / cone
rtl-mc / denot-on-the-fly
rtl-mc / tilextractor
rtl-mc / system-observables
rtl-mc / traverse-operator
rtl-mc / brink
```

---

### Run Tests with Valgrind Enabled

You must have Valgrind installed. Then, run all tests (or a specific test suite) using:

```sh
meson test --wrap='valgrind --leak-check=full --error-exitcode=1' -C ${BUILD_DIR_PATH}
```

**⚠️ NOTE:** Valgrind is incompatible with sanitizers. Make sure to disable them first using:

```sh
meson configure -Db_sanitize=none buildDir
```

References:
- [Meson Test Options](https://mesonbuild.com/Unit-tests.html#other-test-options)
- [How to Use Valgrind with Meson](https://stackoverflow.com/questions/57734973/how-to-use-valgrind-with-meson)