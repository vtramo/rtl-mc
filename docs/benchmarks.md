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

Un esempio di output:
```
-------------------------------------------------------------------------------
PolyhedralSystem Parser benchmarks
-------------------------------------------------------------------------------
../tests/benchmarks/parser/system/system_parser_benchmarks.cpp:6
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
PolyhedralSystem Parser benchmark 1            100             1      16.279 s 
                                        1.24324 us    757.561 ns    3.64414 us 
                                        4.78789 us    14.3642 ns    10.4883 us 
                                                                               

===============================================================================


...


-------------------------------------------------------------------------------
Discretization benchmarks
-------------------------------------------------------------------------------
../tests/benchmarks/discretization/discretization_benchmarks.cpp:7
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Discretization benchmark 1                     100             1    15.7095 ms 
                                        158.552 us    157.119 us    162.348 us 
                                        11.1957 us    5.26598 us    23.9665 us 
                                                                               

==============================================================================

```

### Eseguire una determinata benchmark suite
- `meson test --benchmark -C buildDir -v discretization`
- `meson test --benchmark -C buildDir -v system-parser`
- `meson test --benchmark -C buildDir -v rtlf-parser`
