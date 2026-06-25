FROM ubuntu:26.04

# Package setup
RUN apt update && apt upgrade -y && apt install -y --no-install-recommends \
    ca-certificates \
    clang-22 \
    cmake \
    git \
    libzstd-dev \
    llvm-22-dev \
    ninja-build

RUN ln -s /usr/bin/clang-22 /usr/bin/clang && \
    ln -s /usr/bin/clang++-22 /usr/bin/clang++

# Environment setup
ENV LLVM_DIR=/usr/lib/llvm-22/cmake
ENV Clang_DIR=/usr/lib/llvm-22/lib/cmake/clang
ENV CXX=clang++-22
ENV CC=clang-22
ARG REPO_URL=https://github.com/0152la/InformationFlow

# Get repo
WORKDIR /home/ubuntu
RUN \
    git clone $REPO_URL && \
    cd InformationFlow && \
    git submodule update --init && \
    ./scripts/compile.sh ll
