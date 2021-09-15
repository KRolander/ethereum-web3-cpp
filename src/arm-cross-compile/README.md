# Cross-compile for ARM

This file contains the Makefile for ARM cross-compile, in the Makefile spcify to the variable `GPLUS_TOOLS_PATH` the path of arm-linux-gnueabi-g++ cross-compile tool. 

This file also helps how to cross-compile the Trezor-Crypto, Curl libraries. It must be noted that this Makefile uses only Trezor-Crypto library for transaction signing, because it is more optimal for IoT devices. In the Transaction.c and Transaction.h codes please uncomment the secp256k1 library related lines (inclides and functions). 

It must be noted that all of the cross-compiled librarires (libx.a) must be copied next to the Makefile which is intended to compile the API. 

## Compile Curl for ARM architecture

Compile to obtain a static library (libcurl.a)

```bash
wget http://curl.haxx.se/download/curl-7.37.1.tar.gz
tar xzf curl-7.37.1.tar.gz
mv  curl-7.37.1 curl
cd curl
./configure --prefix=${WORK_SPACE}/build CC=arm-linux-gnueabi-gcc-7 --host=arm-linux-gnueabi --disable-shared --enable-static
```

# Compile Trezor-Crypto for ARM architecture

```bash
cd trezorCrypto
```
In the Makefile modify `CC` = to the path to arm-linux-gnueabi-gcc