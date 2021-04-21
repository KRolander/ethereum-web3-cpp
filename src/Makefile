
# PATH_XKCP=/home/kromes/Documents/Kromes/BlockChain/Ethereum/eth_web3_cpp/XKCP
# PATH_TREZOR_CRYPTO=/home/kromes/Documents/Kromes/BlockChain/Ethereum/eth_web3_cpp/src/crypto


test: test.cpp Transaction.o Util.o Web3.o Payload.o #Keccak-readable-and-compact.o
	g++ -std=c++11 -I. -o test test.cpp Payload.o Web3.o Util.o cJSON.o Transaction.o $(PATH_TREZOR_CRYPTO)/sha3.o $(PATH_TREZOR_CRYPTO)/memzero.o



Transaction.o: Transaction.cpp Web3.o Util.o 
	g++ -std=c++11  -c  Util.o  -I secp256k1/include -I secp256k1/src -I. -L. -lsecp256k1 Transaction.cpp 

Web3.o: Web3.cpp Util.o cJSON.o
	g++ -std=c++11  -c -I. Web3.cpp 

Util.o: Util.cpp
	g++ -std=c++11 -c -I. Util.cpp  

Payload.o: Payload.cpp Transaction.o 
	g++ -std=c++11 -c -I. Payload.cpp

cJSON.o: cJSON.c
	g++ -std=c++11  -c -I. cJSON.c

# Keccak-readable-and-compact.o: $(PATH_XKCP)/Keccak-readable-and-compact.c
# 	gcc -c $(PATH_XKCP)/Keccak-readable-and-compact.c