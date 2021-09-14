# Ethereum web3 cpp

This tool enables interactions with Ethereum Smart Contracts, implemented in C++

The digital signatures of the signatures and batches can be done by using Trezor-Crypto or Secp256k1 libraries. This project uses Trezor-Crypto as default library for signatures (it is a more optimal choice for IoT devices).

In arm-cross-compile file you find a dedicated Makefile and guide for ARM cross-compile.

Please make an issue for questions or errors.

First you have to source the project environment, the Makefile would use the path that is initialized in the setup.sh file.


```bash 
source ./setup.sh
``` 

Second compile cyrptographic tools.

trezor-crypto
```bash 
cd src/crypto
make
``` 

secp256k1 from Bitcoin source, probably should use a more optimal library

```bash 
cd src/secp256k1
make
``` 
Copy the static library secp256k1.a to src.


Third compile the program (test.cpp)

```bash
cd src 
make
```
