ARG UBUNTU_VERSION=24.04
ARG ANTLR4_VERSION=4.13.2
ARG PPL_VERSION=1.2
ARG GMP_VERSION=6.3.0
ARG SPOT_VERSION=2.12.2

FROM ubuntu:${UBUNTU_VERSION} AS build-stage

ARG PPL_VERSION
ARG ANTLR4_VERSION
ARG SPOT_VERSION

RUN apt update
RUN apt install -y cmake=3.28.3-1build7
RUN apt install -y make=4.3-4.1build2
RUN apt install -y wget=1.21.4-1ubuntu4.1
RUN apt install -y pkg-config=1.8.1-2build1
RUN apt install -y libgmpxx4ldbl=2:6.3.0+dfsg-2ubuntu6
RUN apt install -y libgmp-dev=2:6.3.0+dfsg-2ubuntu6
RUN apt install -y libgmp3-dev=2:6.3.0+dfsg-2ubuntu6
RUN apt install -y m4=1.4.19-4build1
RUN apt install -y unzip=6.0-28ubuntu4.1
RUN apt install -y git
RUN apt install -y g++
RUN apt install -y libtool=2.4.7-7build1
RUN apt install -y autoconf=2.71-3
RUN apt install -y automake=1:1.16.5-1.3ubuntu1

# Install PPL dev (thread-safe, sspeed)
RUN git clone https://github.com/BUGSENG/PPL.git
RUN cd /PPL && autoreconf --force --install && \
    ./configure --enable-optimization=sspeed --enable-thread-safe --disable-documentation && \
    make && make install

# ppl.pc
RUN mkdir -p /usr/local/lib/pkgconfig && \
    echo "includedir=/usr/local/include\n\
libdir=/usr/local/lib\n\
\n\
Name: PPL - Parma Polyhedra Library\n\
Description: PPL\n\
URL: https://www.bugseng.com/content/parma-polyhedra-library\n\
Version: ${PPL_VERSION}\n\
Cflags: -I\${includedir}\n\
Libs: -L\${libdir} -L\${libdir}/ppl -lppl -lppl_c" > /usr/local/lib/pkgconfig/ppl.pc

# ANTLR4 Runtime
RUN mkdir antlr4-cpp-runtime-${ANTLR4_VERSION}-source && cd antlr4-cpp-runtime-${ANTLR4_VERSION}-source && \
    wget https://www.antlr.org/download/antlr4-cpp-runtime-${ANTLR4_VERSION}-source.zip && \
    unzip antlr4-cpp-runtime-${ANTLR4_VERSION}-source.zip && \
    mkdir build && mkdir run && cd build && \
    cmake .. && \
    make && make install && cd /usr/local/include/antlr4-runtime && mv * .. && cd .. && rm -rf antlr4-runtime

# ANTLR4 CLI
RUN apt install -y python3-pip=24.0+dfsg-1ubuntu1.1 && python3 -m pip install --break-system-packages 'antlr4-tools==0.2.1'
RUN apt install -y openjdk-11-jdk

# antlr4-runtime.pc
RUN mkdir -p /usr/local/lib/pkgconfig && \
    echo "prefix=/usr/local\n\
includedir=\${prefix}/include\n\
exec_prefix=\${prefix}\n\
libdir=\${exec_prefix}/lib\n\
\n\
Name: ANTLR4 Runtime\n\
Description:\n\
URL: https://www.antlr.org/\n\
Version: ${ANTLR4_VERSION}\n\
Cflags: -I\${includedir}\n\
Libs: -L\${libdir} -lantlr4-runtime" > /usr/local/lib/pkgconfig/antlr4-runtime.pc

# Spot library
RUN wget -q http://www.lrde.epita.fr/dload/spot/spot-${SPOT_VERSION}.tar.gz && \
    tar -xvzf spot-${SPOT_VERSION}.tar.gz && \
    cd spot-${SPOT_VERSION} && \
    ./configure && \
    make && \
    make install

# Install meson
RUN python3 -m pip install meson --break-system-packages
RUN apt install ninja-build -y

WORKDIR /project
COPY . .
ENTRYPOINT ["sh"]

FROM build-stage AS compile-test
RUN apt update
RUN apt install -y valgrind
RUN meson setup  \
    --errorlogs  \
    -Denable_tests=true  \
    -Ddebug=true  \
    -Dgenerate_parser=true  \
    -Denable_profiling=true  \
    buildDir
RUN meson compile -C buildDir -v
RUN meson test -C buildDir -t 15 -v


FROM build-stage AS compile-release
RUN meson setup  \
    --optimization 3  \
    --errorlogs  \
    --buildtype release  \
    -Denable_tests=false  \
    -Ddebug=false  \
    -Dgenerate_parser=true  \
    -Denable_profiling=false  \
    buildDir
RUN meson compile -C buildDir -v

RUN echo "/usr/lib/$(gcc -dumpmachine)" > /lib-path-dir.txt
RUN mkdir -p /libs
RUN find / -regextype posix-extended -regex ".*/lib/.*libppl\.(a|so).*" -exec mv {} /libs/ \;
RUN find / -regextype posix-extended -regex ".*/lib/.*libspot\.(a|so).*" -exec mv {} /libs/ \;
RUN find / -regextype posix-extended -regex ".*/lib/.*libantlr4-runtime\.(a|so).*" -exec mv {} /libs/ \;
RUN find / -regextype posix-extended -regex ".*/lib/.*libgmp.*\.(a|so).*" -exec mv {} /libs/ \;
RUN find / -regextype posix-extended -regex ".*/lib/.*libgmpxx\.(a|so)\.*" -exec mv {} /libs/ \;
RUN find / -regextype posix-extended -regex ".*/lib/.*libbddx\.(a|so).*" -exec mv {} /libs/ \;


FROM ubuntu:${UBUNTU_VERSION}

COPY --from=compile-release /lib-path-dir.txt /lib-path-dir.txt
COPY --from=compile-release /libs /libs
RUN LIB_PATH_DIR=$(cat /lib-path-dir.txt) && mkdir -p ${LIB_PATH_DIR} && mv /libs/* ${LIB_PATH_DIR}
COPY --from=compile-release /project/buildDir/src/rtl-mc /usr/local/bin
COPY --from=compile-release /project/buildDir/tools/rtl-gen/src/rtl-gen /usr/local/bin

ENTRYPOINT ["/bin/bash", "-c"]