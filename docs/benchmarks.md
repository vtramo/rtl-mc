### Come eseguire i benchmarks
Catch2 fornisce [basic micro-benchmarking features](https://github.com/catchorg/Catch2/blob/devel/docs/benchmarks.md).
Per eseguire i benchmark è necessario prima installare i file di input che i benchmarks usano eseguendo
questo comando:
```
meson install --tags tests -C buildDir
```
Successivamente, per eseguire solo i benchmarks, lanciare questo comando:
```
meson test --benchmark -C buildDir
```
