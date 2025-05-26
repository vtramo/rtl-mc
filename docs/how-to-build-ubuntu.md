# Ubuntu 24.04 Setup

### 1. Install **meson**
```bash
sudo apt install meson
```
or
```bash
pip3 install --user meson
```

### 2. Install **pkg-config**
```bash
sudo apt install -y pkg-config
```

### 3. Install **GMP**
```bash
sudo apt install -y libgmp3-dev
```

### 4. Install **PPL** (Parma Polyhedra Library)
```bash
sudo apt-get install libtool autoconf automake
git clone https://github.com/BUGSENG/PPL.git
cd ./PPL
autoreconf --force --install
./configure --enable-optimization=sspeed --enable-thread-safe --disable-documentation
sudo make
sudo make install
```

To find where PPL was installed:
```bash
ppl-config --libdir
```

Example output:
```
/usr/lib/x86_64-linux-gnu
```

Create a `ppl.pc` file in `/usr/lib/x86_64-linux-gnu/pkgconfig` (adjust the path if needed):
```
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

---

### 5. Install **ANTLR4 CLI**
Requires JDK >= 11:
```bash
sudo apt-get install -y openjdk-11-jdk
```

Install CLI via pip:
```bash
sudo apt-get install -y python3-pip
python3 -m pip install --break-system-packages antlr4-tools
```

---

### 6. Install **ANTLR4 Runtime**

If CMake is not installed:
```bash
sudo apt install -y cmake
```

Download the C++ runtime from [ANTLR's website](https://www.antlr.org/download/antlr4-cpp-runtime-4.13.2-source.zip), unzip it, then:
```bash
mkdir build && mkdir run && cd build
sudo cmake ..
sudo make && sudo make install
```

Then clean up the include path:
```bash
cd /usr/local/include/antlr4-runtime && mv * .. && cd .. && rm -rf antlr4-runtime
```

Create `antlr4-runtime.pc` in `/usr/local/lib/pkgconfig`:
```
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

---

### 7. Install **SPOT**
Install from the official repository:
```bash
wget -q -O - https://www.lrde.epita.fr/repo/debian.gpg | sudo apt-key add -
echo 'deb http://www.lrde.epita.fr/repo/debian/ stable/' | sudo tee -a /etc/apt/sources.list
sudo apt-get update
sudo apt-get install spot libspot-dev spot-doc python3-spot
```

---

### 8. Build the project

Configure and build with Meson:
```bash
meson setup buildDir
meson compile -C buildDir
```

To run the tests:
```bash
meson test -C buildDir
```

To run the benchmarks:
```bash
meson test -C buildDir --benchmark
```
