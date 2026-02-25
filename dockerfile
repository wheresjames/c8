FROM debian:stable-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    build-essential \
    git \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -S . -B build \
 && cmake --build build