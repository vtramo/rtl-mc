# RTL Model Checking
Please refer to the documentation in the [docs directory](https://github.com/vtramo/rtl-mc/tree/main/docs) for more details on the prerequisites and how to build the project.

# Docker
If you have Docker installed...

1. Build the image (it takes a few minutes...)
    ```shell
    docker build -t rtl-mc .
    ```
2. Run the container
    ```shell
    docker run --rm rtl-mc
    ```
   ```
    formula: 1 argument(s) expected. 0 provided.
    Usage: rtl-mc [--help] [--version] [[--low]|[--medium]|[--high]] [--any] system formula
    
    Positional arguments:
    system         Polyhedral System file
    formula        RTLf file
    
    Optional arguments:
    -h, --help     shows help message and exits
    -v, --version  prints version information and exits
    --low          minimal optimizations (fast, default)
    --medium       moderate optimizations
    --high         all available optimizations (slow)
    --any          tells the translator that it should attempt to reduce or produce a deterministic result: any automaton denoting the given formula is OK.  This effectively disables post-processings and speeds up the translation
   ```
You need to provide a polyhedral system file and an rtlf file. You can do this by creating bind mounts:
```shell
docker run \
  --rm \
  -v ./examples/GAP_k=1/system.txt:/system.txt \
  -v ./examples/GAP_k=1/rtlf.txt:/rtlf.txt \
  rtl-mc system.txt rtlf.txt
```
```
POLYHEDRAL SYSTEM: 
Inv ( { b >= 0 & a >= 0 } )
Flow { t = 1 & -a - b >= -2 & -a >= -1 & -b >= -2 & b >= -2 & a >= -1 & a + b >= -2 }

p ( { a - b >= 1 & b >= 0 } )
q ( { -a + b >= 1 & a >= 0 } )
t0 ( { t = 0 & b >= 0 & a >= 0 } )
t1 ( { -t >= -10 & b >= 0 & a >= 0 } )


RTLf Formula: t0 & Gt1 & F(p & Fq)
DiscreteLtlFormula: alive & sing & t0 & (alive U G!alive) & G(!alive | !Xalive | (sing <-> X(alive & !sing))) & F(alive & sing & !Xalive) & G(!alive | t1) & F(alive & p & F(alive & q))

Total states: 20
Total edges: 37
Initial initial states: 4
Total final states: 5
Denot total iterations: 14
RESULT ( { t = 0 & b >= 0 & a - b >= 1 & -a + b > -29 } { t = 0 & -a + b > -1 & b >= 0 & a >= 0 & a - b > -27 } )
```