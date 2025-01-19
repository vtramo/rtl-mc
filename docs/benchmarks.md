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
- `meson test --benchmark -C buildDir -t 0 -v automaton-construction` (`-t 0` per disabilitare il timeout)
- `meson test --benchmark -C buildDir -t 0 -v denot` (`-t 0` per disabilitare il timeout)

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
Automaton construction benchmarks (formula in input quella del paper)
-------------------------------------------------------------------------------
../tests/benchmarks/automaton/automaton_benchmarks.cpp:10
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Automaton construction benchmark 1             100             1    370.971 ms 
                                        3.56679 ms    3.55703 ms    3.58146 ms 
                                        60.1448 us    44.7601 us    87.6618 us 
                                                                               

===============================================================================


-------------------------------------------------------------------------------
Denot benchmark GAP
-------------------------------------------------------------------------------
../tests/benchmarks/denot/denot_benchmarks.cpp:14
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
k=50                                           100             1     2.26753 m 
                                          1.3522 s     1.34968 s     1.35522 s 
                                        14.0225 ms    11.9017 ms     16.699 ms 
                                                                               

===============================================================================
```
