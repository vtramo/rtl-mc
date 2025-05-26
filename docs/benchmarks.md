### How to Run Benchmarks
The project uses [Catch2's basic micro-benchmarking features](https://github.com/catchorg/Catch2/blob/devel/docs/benchmarks.md).
To run **only** the benchmarks (skipping unit tests), use the following command:
```
meson test --benchmark -C buildDir -v
```
Before running benchmarks, make sure sanitizers are disabled (they are off by default) with:
```
meson configure -Db_sanitize=none buildDir
```
Then rebuild the project. Otherwise, the benchmark results may not be accurate.
### How to List Available Benchmarks
```
meson test -C buildDir --benchmark --list
```
### Run a Specific Benchmark Suite
- `meson test --benchmark -C buildDir -v discretisation`
- `meson test --benchmark -C buildDir -v system-parser`
- `meson test --benchmark -C buildDir -v rtlf-parser`
- `meson test --benchmark -C buildDir -t 0 -v automaton-construction`
- `meson test --benchmark -C buildDir -t 0 -v denot-on-the-fly`
- `meson test --benchmark -C buildDir -t 0 -v tilextractor`
> **Note:** The `-t 0` flag disables the timeout for long-running benchmarks.