//
//   Author: Roland Kromes, @ roland.kromes@univ-cotedazur.fr
//   Address: LEAT Laboratory, Université Côte d'Azur
//
//   The Transaction class is used for creating the transaction sceleton and creating the format
//   allowing the interaction with the Smart Contracts. This class contains also the send Transaction function 
//   finalizing the Tx format.
//
//    TODO : add curl using Web3js format
//  
//    

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "Web3.h"
#include <vector>

#include "crypto/sha3.h"
#include "Util.h"


using namespace std;

class Transaction {

public:
    typedef struct {
        char from[80];
        char to[80];
        char gasPrice[20];
        long gas;
    } Options;
    Options options;

public:
    Transaction(Web3* _web3, const string* address);
    void SetPrivateKey(const uint8_t *key);
    string SetupContractData(const string *func, ...);
    string Call(const string* param);
    string SendTransaction(uint32_t nonceVal, uint64_t gasPriceVal, uint32_t gasLimitVal,
                           string *toStr, string *valueStr, string *dataStr);
    
    vector<uint8_t> RlpEncode_v2(
            uint32_t nonceVal, uint32_t gasPriceVal, uint32_t  gasLimitVal,
            string* toStr, string* valueStr, string* dataStr);
    void HexStrToUchar(unsigned char *dest, const char *source, int bytes_n);
    int chhex(char ch);
private:
    // Log Debug;
    // #define LOG(x) Debug.println(x)

    Web3* web3;
    const string * contractAddress;
    const uint8_t * privateKey;

private:
    string GenerateContractBytes(const string *func);
    string GenerateBytesForInt(const int32_t value);
    string GenerateBytesForUint(const uint32_t value);
    string GenerateBytesForAddress(const string *value);
    string GenerateBytesForString(const string *value);
    string GenerateBytesForBytes(const char *value, const int len);

    void GenerateSignature(uint8_t* signature, int* recid, uint32_t nonceVal, uint64_t gasPriceVal, uint32_t  gasLimitVal,
                           string* toStr, string* valueStr, string* dataStr);
    vector<uint8_t> RlpEncode(
            uint32_t nonceVal, uint64_t gasPriceVal, uint32_t  gasLimitVal,
            string* toStr, string* valueStr, string* dataStr);
    vector<uint8_t> RlpEncodeForRawTransaction(
            uint32_t nonceVal, uint64_t gasPriceVal, uint32_t  gasLimitVal,
            string* toStr, string* valueStr, string* dataStr, uint8_t* sig, uint8_t recid);
    void Sign(uint8_t* hash, uint8_t* sig, int* recid);
};


#endif //TRANSACTION_H
