//
//   Author: Roland Kromes, @ roland.kromes@univ-cotedazur.fr
//   Address: LEAT Laboratory, Université Côte d'Azur
//
//   The Util class contains tools for RLP encoding and hexa string representation encoding
//

#include "Payload.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdint.h>

#include <string.h>
#include <cstring>

#define PRIVATE_KEY_SIZE 32
#define SHA3_256_DIGEST_LENGTH 32
#define SIGNATURE_LENGTH 64

#define INFURA_HOST "http://127.0.0.1:7545"
#define INFURA_PATH "/<YOUR_INFURA_ID>"

#define CONTRACT_ADDRESS "0x8668"

// char HexLookUp[] = "0123456789abcdef";    
// void Payload::bytes2hex (unsigned char *src, char *out, int len)
// {
//     while(len--)
//     {
//         *out++ = HexLookUp[*src >> 4];
//         *out++ = HexLookUp[*src & 0x0F];
//         src++;
//     }
//     *out = 0;
// }


int Payload::chhex(char ch)
{
    if (isdigit(ch))
        return ch - '0';
    if (tolower(ch) >= 'a' && tolower(ch) <= 'f')
        return ch - 'a' + 10;
    return -1;
}

void Payload::HexStrToUchar(unsigned char *dest, const char *source, int bytes_n)
{
    for (bytes_n--; bytes_n >= 0; bytes_n--)
        dest[bytes_n] = 16 * chhex(source[bytes_n * 2]) + chhex(source[bytes_n * 2 + 1]);
}

// data is a hexstring it corresponds to uint256 type of Solidity
// !!! Attention !!! it must be noted that we supposed that data is uint256, it can be other
// type or even types e.g uint8[6] and bytes32, in that case the two 
// variablse must be concatenated into a uint8_t. Another thing to note
// the data concatenation must be done in the same way in the Smart Contract
// else the signature could not be verified 

void Payload::signedPayload(std::string data, uint8_t ID, std::string privKey){

    // string to uint8_t
    uint8_t privateKeyBytes[PRIVATE_KEY_SIZE];
    HexStrToUchar(privateKeyBytes, &privKey[0], PRIVATE_KEY_SIZE);
   
    
    // data is a hexstring 
    uint8_t dataBytes[data.size()/2];
    HexStrToUchar(dataBytes, &data[0], data.size());
    

    // // Hash(data || ID) (Keccak-256) 
    int iterate = data.size()/2 + 1;
    uint8_t dataToHash[iterate];
    int i;

   
    for(i=0; i< iterate-1; i++)
    {
         dataToHash[i] = (uint8_t) dataBytes[i];
    }
    dataToHash[iterate-1] = ID;

    uint8_t hashDigest[SHA3_256_DIGEST_LENGTH];
    
    keccak_256(dataToHash, iterate, hashDigest);

    char out_c[65]; // 32 * 2 + 1
    Util::bytes2hex(hashDigest, out_c, 32);
    std::string out_hash(out_c);

    // TODO signe here

    Web3 web3((std::string *)INFURA_HOST, (std::string *)INFURA_PATH);
    Transaction transaction(&web3, (std::string *)CONTRACT_ADDRESS);

    transaction.SetPrivateKey(privateKeyBytes);

    uint8_t signature[SIGNATURE_LENGTH];
    int recid[1] = {0};

    transaction.Sign(hashDigest, signature, recid);

    char out_char_signature[129]; // 64 * 2 + 1
    Util::bytes2hex(signature, out_char_signature, 64);
    std::string out_signature(out_char_signature);    

    r = out_signature;
    s = "hy";

}

void Payload::Test(){
    r = "IronMan";
    s = "AntMan";
}


