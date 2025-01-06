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
- `meson test --benchmark -C buildDir -v discretization`
- `meson test --benchmark -C buildDir -v system-parser`
- `meson test --benchmark -C buildDir -v rtlf-parser`
- `meson test --benchmark -C buildDir -t 2 -v automaton-construction` (`-t 2` per incrementare il timeout!!)

### Risultati attuali
Eseguiti su una macchina con processore 13th Gen Intel(R) Core(TM) i9-13900KF e 64GB di RAM
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



-------------------------------------------------------------------------------
Discretization benchmarks
-------------------------------------------------------------------------------
../tests/benchmarks/discretization/discretization_benchmarks.cpp:7
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Discretization benchmark 1                     100             1    272.996 ms 
                                         2.7611 ms    2.75646 ms    2.76591 ms 
                                        24.2133 us    20.8521 us    29.8995 us 
                                                                               

===============================================================================



-------------------------------------------------------------------------------
RTLf Parser benchmarks
-------------------------------------------------------------------------------
../tests/benchmarks/parser/rtlf/rtlf_parser_benchmarks.cpp:7
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
RTLf Parser benchmark 1                        100             1    20.0518 ms 
                                        201.285 us    200.498 us    203.515 us 
                                         6.2494 us    2.80045 us    13.4992 us 
                                                                               

===============================================================================


-------------------------------------------------------------------------------
Automaton construction benchmarks
-------------------------------------------------------------------------------
../tests/benchmarks/automaton/automaton_benchmarks.cpp:10
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Automaton construction benchmark 1             100             1     28.0814 s 
                                          285.3 ms    284.512 ms    286.594 ms 
                                         5.0559 ms    3.59694 ms    8.36769 ms 
                                                                               

==============================================================================
```
