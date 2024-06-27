# Build stage
FROM ubuntu:22.04
LABEL Description="Build environment"

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \
    clang \
    cmake \
    g++

WORKDIR /udp

COPY . .

RUN cmake -S . -B build && cmake --build build && cmake --install build

WORKDIR ./build/bin

CMD ["sh", "-c", "./server"]