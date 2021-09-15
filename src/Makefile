
# PATH_XKCP=/home/kromes/Documents/Kromes/BlockChain/Ethereum/eth_web3_cpp/XKCP
# PATH_TREZOR_CRYPTO=/home/kromes/Documents/Kromes/BlockChain/Ethereum/eth_web3_cpp/src/crypto



SRCS   = crypto/bignum.c crypto/ecdsa.c crypto/curves.c crypto/secp256k1.c crypto/nist256p1.c crypto/rand.c crypto/hmac.c crypto/bip32.c crypto/bip39.c crypto/pbkdf2.c crypto/base58.c crypto/base32.c
SRCS  += crypto/address.c
SRCS  += crypto/script.c
SRCS  += crypto/ripemd160.c
SRCS  += crypto/sha2.c
SRCS  += crypto/sha3.c
SRCS  += crypto/hasher.c
SRCS  += crypto/aes/aescrypt.c crypto/aes/aeskey.c crypto/aes/aestab.c crypto/aes/aes_modes.c
SRCS  += crypto/ed25519-donna/curve25519-donna-32bit.c crypto/ed25519-donna/curve25519-donna-helpers.c crypto/ed25519-donna/modm-donna-32bit.c
SRCS  += crypto/ed25519-donna/ed25519-donna-basepoint-table.c crypto/ed25519-donna/ed25519-donna-32bit-tables.c crypto/ed25519-donna/ed25519-donna-impl-base.c
SRCS  += crypto/ed25519-donna/ed25519.c crypto/ed25519-donna/curve25519-donna-scalarmult-base.c crypto/ed25519-donna/ed25519-sha3.c crypto/ed25519-donna/ed25519-keccak.c
SRCS  += crypto/monero/base58.c
SRCS  += crypto/monero/serialize.c
SRCS  += crypto/monero/xmr.c
SRCS  += crypto/monero/range_proof.c
SRCS  += crypto/blake256.c
SRCS  += crypto/blake2b.c crypto/blake2s.c
SRCS  += crypto/chacha_drbg.c
SRCS  += crypto/groestl.c
SRCS  += crypto/chacha20poly1305/chacha20poly1305.c crypto/chacha20poly1305/chacha_merged.c crypto/chacha20poly1305/poly1305-donna.c crypto/chacha20poly1305/rfc7539.c
SRCS  += crypto/rc4.c
SRCS  += crypto/nem.c
SRCS  += crypto/segwit_addr.c crypto/cash_addr.c
SRCS  += crypto/memzero.c
SRCS  += crypto/shamir.c
SRCS  += crypto/hmac_drbg.c
SRCS  += crypto/rfc6979.c
SRCS  += crypto/slip39.c

OBJS   = $(SRCS:.c=.o)


#$(PATH_TREZOR_CRYPTO)/sha3.o $(PATH_TREZOR_CRYPTO)/memzero.o
test: test.cpp Transaction.o Util.o Web3.o Payload.o #Keccak-readable-and-compact.o
	g++ -std=c++11 -I. -o test test.cpp Payload.o Web3.o Util.o cJSON.o Transaction.o $(OBJS) -lcurl



Transaction.o: Transaction.cpp Web3.o Util.o 
	g++ -std=c++11  -c Util.o -I secp256k1/include -I secp256k1/src -I. -L. -lsecp256k1 Transaction.cpp 

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