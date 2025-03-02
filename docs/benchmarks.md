### Come eseguire i benchmarks
Catch2 fornisce [basic micro-benchmarking features](https://github.com/catchorg/Catch2/blob/devel/docs/benchmarks.md).

Per eseguire solo e tutti i benchmarks (skip test di unità), lanciare questo comando:
```
meson test --benchmark -C buildDir -v
```
Prima di eseguire i benchmark, disattivare i sanitizers con questo comando (disattivati di default):
```
meson configure -Db_sanitize=none buildDir
```
e ricompilare (altrimenti i risultati non saranno "precisi").

### Eseguire una determinata benchmark suite
- `meson test --benchmark -C buildDir -v discretisation`
- `meson test --benchmark -C buildDir -v system-parser`
- `meson test --benchmark -C buildDir -v rtlf-parser`
- `meson test --benchmark -C buildDir -t 0 -v automaton-construction` (`-t 0` per disabilitare il timeout)
- `meson test --benchmark -C buildDir -t 0 -v denot-on-the-fly`