SDK
===

[![Build Status](https://travis-ci.org/SylvestreG/sdk.png)](https://travis-ci.org/SylvestreG/sdk)

## Overview

hls is a c++ HLS proxy

## Installation

### Requirements
* [Required] - [Boost (1.53 or newer)](http://www.boost.org/)
* [Required MacOSX] - [restclient-cpp]

Installing dependencies on MacOsX, you need brew

```bash
$ brew tap mrtazz/oss
$ brew install restclient-cpp
$ brew install boost
```

For Linux you only need boost

for ArchLinux
```bash
$ pacman -S boost
```

for Ubuntu
```bash
apt-get install libboost-all-dev
```
### Building

```bash
$ git clone https://github.com/SylvestreG/sdk.git
$ cd sdk && mkdir build && cd build
$ cmake .. && make
```

## How to use

launch hls with the url you want to proxy as argument.

### Building

```bash
$ ./sdk -u https://test-streams.mux.dev/x36xhzz/x36xhzz.m3u8
```

After that you can open VLC, and go to File > Open Network...
put this url in vlc http://localhost:8080/hello.m3u8

if you want sdk to bind on another port you can, if you want
to bind on port 4242 :
```bash
$ ./sdk -p 4242 -u https://test-streams.mux.dev/x36xhzz/x36xhzz.m3u8
```
