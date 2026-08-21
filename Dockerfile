### Thin build-environment image for IEC60730-Library
### Firmware toolchains and Simplicity SDK are installed later by SLT (make bootstrap).

FROM ubuntu:24.04

ENV TZ=Europe/Budapest
ENV DEBIAN_FRONTEND=noninteractive

# Install only the OS-level prerequisites needed to bootstrap SLT and run the
# project scripts. Firmware toolchains and SDKs are installed by SLT.
RUN apt-get update \
    && apt-get install --no-install-recommends -y \
    build-essential \
    bzip2 \
    ca-certificates \
    ccache \
    curl \
    git \
    git-lfs \
    gpg \
    jq \
    libgl1 \
    libglib2.0-0 \
    libpcre2-dev \
    make \
    ninja-build \
    openjdk-21-jre-headless \
    python3 \
    python3-pip \
    python3-setuptools \
    srecord \
    unzip \
    wget \
    zip \
    && rm -rf /var/lib/apt/lists/*

ENV PATH="${PATH}:/usr/local/bin"

WORKDIR /workspace
