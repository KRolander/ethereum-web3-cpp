# ethereum-web3-cpp
This tool enables interactions with Ethereum Smart Contracts, implemented in C++

- First you have to source the project environment, the Makefile would use the path that is initialized in the setup.sh file

```bash 
source ./setup.sh
``` 

- Second compile cyrptographic tools

trezor-crypto
```bash 
cd crypto
make
``` 

secp256k1 from Bitcoin source, probably sgould use a more optimal library
```bash 
cd secp256k1
make
``` 

- Third compile the program (test.cpp)

```bash 
make
```