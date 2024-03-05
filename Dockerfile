FROM ubuntu:22.04
LABEL authors="风唤长河"

RUN mv /etc/apt/sources.list /etc/apt/sources.list.bak
COPY sources.list /etc/apt/

RUN apt-get update && apt-get install -y \
    gcc \
    cmake \
    g++ \
    git \
    gdb \
    llvm \
    clang \
    valgrind \
    openssh-server \
    zlib1g-dev \
    net-tools \
    sudo \
    vim \
    flex \
    bison




