# Ubuntu 24.04
### 1. Installare **meson**:
```
sudo apt install meson
```
oppure
```
pip3 install --user meson
```
### 2. Installare **pkg-config**:
```
sudo apt install -y pkg-config
```
### 3. Installare **GMP**
```
sudo apt install -y libgmp3-dev
```

### 4. Installare **PPL** (Parma Polyhedral Library)
```
sudo apt install -y libppl-dev ppl-dev
```
individuare dove è stato installato PPL:
```
ppl-config --libdir
```
nel mio caso l'output è:
```
/usr/lib/x86_64-linux-gnu
```
Creare un pkg config file chiamato `ppl.pc` in `/usr/lib/x86_64-linux-gnu/pkgconfig` (cambiare il percorso in base all'output del precedente comando):
```
prefix=/usr
includedir=${prefix}/include/x86_64-linux-gnu
exec_prefix=${prefix}
libdir=${exec_prefix}/lib/x86_64-linux-gnu

Name: PPL - Parma Polyhedra Library
Description: PPL
URL: https://www.bugseng.com/content/parma-polyhedra-library
Version: 14.0.0
Cflags: -I${includedir}
Libs: -L${libdir} -L${libdir}/ppl -lppl -lppl_c
```
questo pkg config file verrà letto da meson per individuare la dipendenza sul sistema.

### 6. Installare ANTLR4 CLI
```
sudo apt install -y antlr4
```
oppure
```
pip3 install antlr4-tools
```

### 7. Installare ANTLR4 Runtime
Installare CMake se assente:
```
sudo apt install -y cmake
```
Scaricare da [qui](https://www.antlr.org/download/antlr4-cpp-runtime-4.13.2-source.zip) il codice sorgente C++.

Successivamente, entrare nella directory del progetto ed eseguire:
```
mkdir build && mkdir run && cd build
sudo cmake .. 
sudo make && sudo make install
```
Nella cartella `build` si troverà il risultato della compilazione. Successivamente, creare un pkg config file nella stessa
directory dove si è installato PPL con il nome `antlr4-runtime.pc`:
```
prefix=/usr/local/antlr4-runtime
includedir=${prefix}/include
exec_prefix=${prefix}
libdir=${exec_prefix}/lib

Name: ANTLR4 Runtime
Description:
URL: https://www.antlr.org/
Version: 4.13.2
Cflags: -I${includedir}
Libs: -L${libdir} -lantlr4-runtime
```
### 8. Installare spot
Attualmente usiamo una nostra versione modificata di spot (per introdurre l'operatore Strict Until).
Clonare tale versione di spot da [https://github.com/vtramo/spot](https://github.com/vtramo/spot).
```
git clone https://github.com/vtramo/spot
```
Fare il checkout nel branch `mc/rtlf`:
```
git checkout mc/rtlf
```

Successivamente, sono necessari una serie di tools per buildare spot:
```
sudo apt-get install -y autoconf automake libtool flex bison emacs org-mode groff swig doxygen r-base perl latexmk texlive-fonts-extra  wget pdf2svg imagemagick jupyter graphviz
```
E' anche necessario:
- Java >= 1.7
- Python >= 3.8, IPython >= 2.3
- Jupyter con nbconvert

Per maggiori dettagli, leggere `HACKING`.

Successivamente, eseguire i seguenti comandi:
```
autoreconf -vfi
./configure && sudo make && sudo make install
```
### 9. Compilare il progetto
Compilare il progetto con meson:
```
meson setup buildDir
meson compile -C buildDir
```
Per eseguire i test:
```
meson test -C buildDir
```
Per eseguire i benchmarks:
```
meson test -C buildDir --benchmark
```