# Multi-stage build for massive-cpp
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy source
WORKDIR /src
COPY . .

# Build
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release -DMASSIVE_BUILD_TESTS=OFF
RUN cmake --build build -j$(nproc)

# Runtime stage
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libssl3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /src/build/libmassive* /usr/local/lib/
COPY --from=builder /src/include /usr/local/include

ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

