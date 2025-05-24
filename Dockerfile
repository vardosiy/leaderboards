FROM ubuntu:24.04

RUN apt-get update && apt install -y software-properties-common wget make

# GCC 11.3.0
RUN add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y gcc-11 g++-11


# CMake 3.22.1
RUN wget https://github.com/Kitware/CMake/releases/download/v3.22.1/cmake-3.22.1-linux-x86_64.sh && \
    chmod +x cmake-3.22.1-linux-x86_64.sh && \
    ./cmake-3.22.1-linux-x86_64.sh --skip-license --prefix=/usr && \
    rm cmake-3.22.1-linux-x86_64.sh

# vcpkg
RUN apt-get install -y \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    ninja-build

ENV VCPKG_FORCE_SYSTEM_BINARIES=1
RUN git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg && \
    /opt/vcpkg/bootstrap-vcpkg.sh -disableMetrics

RUN apt-get install -y gdb

# Clean up
RUN apt-get clean && rm -rf /var/lib/apt/lists/*

COPY . /app

WORKDIR /app
CMD ["/bin/bash"]
