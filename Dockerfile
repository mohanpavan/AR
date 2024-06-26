# Build stage
FROM ubuntu:22.04
LABEL Description="Build environment"

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \
    clang \
    cmake \
    g++

# Copy the entire project
COPY . /app

# Build the project
WORKDIR /app
RUN mkdir build_dock && cd build_dock && cmake .. && make

# CMD ["ls -l ./build_dock/server", "./build_dock/server/server"]
