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
Parser benchmarks
-------------------------------------------------------------------------------
../tests/benchmarks/parser/parser_benchmarks.cpp:6
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Parser benchmark 1                             100             5    1.11601 ms 
                                        2.41365 us    2.29888 us    2.90215 us 
                                        1.03211 us    190.412 ns    2.41793 us 
                                                                               

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
- `meson test --benchmark -C buildDir -v discretization-benchmarks`
- `meson test --benchmark -C buildDir -v parser-benchmarks`
