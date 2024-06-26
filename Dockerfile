# Build stage
FROM ubuntu:22.04
LABEL Description="Build environment"

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \
    clang \
    cmake \
    g++

COPY . /udp

WORKDIR /udp

RUN cmake -S . -B build && cmake --build build && cmake --install build
