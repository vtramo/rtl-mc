## Discretisation of an RTLf Formula

An RTLf formula undergoes a series of transformations, including:

- **1. Discretisation according to the paper rules**: the RTLf formula is discretised and transformed into an LTLf formula;
- **2. Conversion to an LTL formula**: the resulting LTLf formula is then transformed into an LTL formula.

These transformations can likely be applied in a single pass over the formula tree (using Spot), reducing the number of required steps.

Spot provides a `spot::from_ltlf` function to convert a formula with LTLf semantics into a formula with LTL semantics.

There are two implementations to handle the above transformations:

1. An implementation that performs both discretisation and LTL conversion in one go (does not use `spot::from_ltlf`, but implements the conversion from scratch — `--direct-ltl`).
2. An implementation that discretises first, then uses `from_ltlf` to convert to LTL (default).

Both versions have been tested. Version 1 is more performant:

```
-------------------------------------------------------------------------------
Discretisation benchmarks
-------------------------------------------------------------------------------
../tests/benchmarks/discretisation/discretisation_benchmarks.cpp:7
...............................................................................

-benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
RTLf to LTL (our version)                      100             1    65.3582 ms 
                                        660.581 us    656.526 us    675.324 us 
                                        34.7105 us    10.0423 us    78.7317 us 
                                                                               
RTLf to LTL (spot)                             100             1    160.943 ms 
                                        1.62471 ms    1.62274 ms    1.62705 ms 
                                        10.8612 us    8.49642 us    16.4507 us 
                                                                               

===============================================================================
All tests passed (838 assertions in 1 test case)
```

The transformation rules for each case are implemented under the `rules` directory.