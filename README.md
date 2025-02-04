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
   Usage: rtl-mc [--help] [--version] [[--from-files VAR...]|[--gap VAR...]|
              [--nogap VAR...]] [[--existential]|[--universal]] [--mc VAR]
              [--direct-ltl] [[--low]|[--medium]|[--high]] [--any]
              [[--verbose]...|[--quiet]|[--stats VAR]] [--concurrent]

   Model Checking Linear Temporal Properties on Polyhedral Systems
   
   Optional arguments:
   -h, --help        shows help message and exits
   -v, --version     prints version information and exits
   -f, --from-files  Polyhedral System file and RTLf file (formula φ) [nargs: 2]
   --gap             GAP experiment with k alternating steps and max time t. Example: --gap 3 15. [nargs: 2]
   --nogap           NO GAP experiment with k alternating steps and max time t. Example: --nogap 2 20. [nargs: 2]
   --existential     compute the set of points from which there exists a trajectory that satisfies φ (default)
   --universal       compute the set of points from which every trajectory satisfies φ
   --mc              Check if a given point x ∈ ℝⁿ is the source of a trajectory in the polyhedral system that satisfies the temporal formula φ. For --existential, checks if some trajectory from the point satisfies φ. For --universal, checks if all trajectories from the point satisfy φ. Specify all system variables with integer values (e.g., [x=1, y=2, z=3]).
   --direct-ltl      discretize the RTLf formula directly into LTL in a single step.
   --low             minimal optimizations during automaton construction (fast, default)
   --medium          moderate optimizations during automaton construction
   --high            all available optimizations during automaton construction (slow)
   --any             tells the translator that it should attempt to reduce or produce a deterministic result: any automaton denoting the given formula is OK.  This effectively disables post-processings and speeds up the translation
   -V, --verbose     enable verbose output. Each occurrence of -V increases verbosity level [may be repeated]
   -q, --quiet       suppress all normal output
   -s, --stats       formats the execution statistics. Example: --stats "Tot states: %Ats". Placeholders (%Ats, %Ate, etc.) are described in the documentation.
   -c, --concurrent  concurrent execution (experimental)
   ```
You need to provide a polyhedral system file and an rtlf file. You can do this by creating bind mounts:
```shell
docker run \
  --rm \
  -v ./examples/GAP/system.txt:/system.txt \
  -v ./examples/GAP/rtlf_k=1.txt:/rtlf.txt \
  rtl-mc -f system.txt rtlf.txt
```
```
( { t = 0 & b >= 0 & a - b >= 1 & -a + b >= -29 } { t = 0 & -a + b > -1 & b >= 0 & a >= 0 & a - b >= -27 } )
```