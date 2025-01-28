# Ubuntu 24.04
### 1. Installare **meson**:
```bash
sudo apt install meson
```
oppure
```bash
pip3 install --user meson
```
### 2. Installare **pkg-config**:
```bash
sudo apt install -y pkg-config
```
### 3. Installare **GMP**
```bash
sudo apt install -y libgmp3-dev
```

### 4. Installare **PPL** (Parma Polyhedral Library)
```bash
sudo apt install -y libppl-dev ppl-dev
```
individuare dove è stato installato PPL:
```bash
ppl-config --libdir
```
nel mio caso l'output è:
```text
/usr/lib/x86_64-linux-gnu
```
Creare un pkg config file chiamato `ppl.pc` in `/usr/lib/x86_64-linux-gnu/pkgconfig` (cambiare il percorso in base all'output del precedente comando):
```text
prefix=/usr
includedir=${prefix}/include/x86_64-linux-gnu
exec_prefix=${prefix}
libdir=${exec_prefix}/lib/x86_64-linux-gnu

Name: PPL - Parma Polyhedra Library
Description: PPL
URL: https://www.bugseng.com/content/parma-polyhedra-library
Version: 1.2
Cflags: -I${includedir}
Libs: -L${libdir} -L${libdir}/ppl -lppl -lppl_c
```
questo pkg config file verrà letto da meson per individuare la dipendenza sul sistema.

### 6. Installare ANTLR4 CLI
```bash
sudo apt install -y antlr4
```

oppure:

Necessita di una JDK >= 11:
```bash
sudo apt-get install -y openjdk-11-jdk
```
```bash
sudo apt-get install -y python3-pip
python3 -m pip install --break-system-packages antlr4-tools # preferibile
```

### 7. Installare ANTLR4 Runtime
Installare CMake se assente:
```bash
sudo apt install -y cmake
```
Scaricare da [qui](https://www.antlr.org/download/antlr4-cpp-runtime-4.13.2-source.zip) il codice sorgente C++.

Successivamente, entrare nella directory del progetto ed eseguire:
```bash
mkdir build && mkdir run && cd build
sudo cmake .. 
sudo make && sudo make install
```
Successivamente, eseguire:
```bash
cd /usr/local/include/antlr4-runtime && mv * .. && cd .. && rm -rf antlr4-runtime
```
Infine, creare un pkg config file `antlr4-runtime.pc` in `/usr/local/lib/pkgconfig`:
```text
prefix=/usr/local
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
Installare spot (https://spot.lre.epita.fr/install.html):
```bash
wget -q -O - https://www.lrde.epita.fr/repo/debian.gpg | apt-key add -
echo 'deb http://www.lrde.epita.fr/repo/debian/ stable/' >> /etc/apt/sources.list
apt-get update
apt-get install spot libspot-dev spot-doc python3-spot # Or a subset of those
```
### 9. Compilare il progetto
Compilare il progetto con meson:
```bash
meson setup buildDir
meson compile -C buildDir
```
Per eseguire i test:
```bash
meson test -C buildDir
```
Per eseguire i benchmarks:
```bash
meson test -C buildDir --benchmark
```