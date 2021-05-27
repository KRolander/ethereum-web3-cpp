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
void Payload::hexStringToUint8_t(uint8_t *dest, const char *source, int bytes_n)
{

    int i;
    int j = 0;
    for (i = 0; i < bytes_n; i++)
    {
        if (source[j] == '0')
        {
            dest[i] = 0; //0x00
        }
        else if (source[j] == '1')
        {
            dest[i] = 16; // 0x10
        }
        else if (source[j] == '2')
        {
            dest[i] = 32; // 0x20
        }
        else if (source[j] == '3')
        {
            dest[i] = 48; // 0x30
        }
        else if (source[j] == '4')
        {
            dest[i] = 64; // 0x40
        }
        else if (source[j] == '5')
        {
            dest[i] = 80; // 0x50
        }
        else if (source[j] == '6')
        {
            dest[i] = 96; // 0x60
        }
        else if (source[j] == '7')
        {
            dest[i] = 112; // 0x70
        }
        else if (source[j] == '8')
        {
            dest[i] = 128; // 0x80
        }
        else if (source[j] == '9')
        {
            dest[i] = 144; // 0x90
        }
        else if (source[j] == 'a')
        {
            dest[i] = 160; // 0xa0
        }
        else if (source[j] == 'b')
        {
            dest[i] = 176; // 0xb0
        }
        else if (source[j] == 'c')
        {
            dest[i] = 192; // 0xc0
        }
        else if (source[j] == 'd')
        {
            dest[i] = 208; // 0xd0
        }
        else if (source[j] == 'e')
        {
            dest[i] = 224; // 0xe0
        }
        else if (source[j] == 'f')
        {
            dest[i] = 240; // 0xf0
        }
        else if (source[j] == 'A')
        {
            dest[i] = 160; // 0xa0
        }
        else if (source[j] == 'B')
        {
            dest[i] = 176; // 0xb0
        }
        else if (source[j] == 'C')
        {
            dest[i] = 192; // 0xc0
        }
        else if (source[j] == 'D')
        {
            dest[i] = 208; // 0xd0
        }
        else if (source[j] == 'E')
        {
            dest[i] = 224; // 0xe0
        }
        else if (source[j] == 'F')
        {
            dest[i] = 240; // 0xf0
        }

        j++;

        if (source[j] == '0')
        {
            dest[i] = (dest[i] | 0x00);
        }
        else if (source[j] == '1')
        {
            dest[i] = (dest[i] | 0x01);
        }
        else if (source[j] == '2')
        {
            dest[i] = (dest[i] | 0x02);
        }
        else if (source[j] == '3')
        {
            dest[i] = (dest[i] | 0x03);
        }
        else if (source[j] == '4')
        {
            dest[i] = (dest[i] | 0x04);
        }
        else if (source[j] == '5')
        {
            dest[i] = (dest[i] | 0x05);
        }
        else if (source[j] == '6')
        {
            dest[i] = (dest[i] | 0x06);
        }
        else if (source[j] == '7')
        {
            dest[i] = (dest[i] | 0x07);
        }
        else if (source[j] == '8')
        {
            dest[i] = (dest[i] | 0x08);
        }
        else if (source[j] == '9')
        {
            dest[i] = (dest[i] | 0x09);
        }
        else if (source[j] == 'a')
        {
            dest[i] = (dest[i] | 0x0a);
        }
        else if (source[j] == 'b')
        {
            dest[i] = (dest[i] | 0x0b);
        }
        else if (source[j] == 'c')
        {
            dest[i] = (dest[i] | 0x0c);
        }
        else if (source[j] == 'd')
        {
            dest[i] = (dest[i] | 0x0d);
        }
        else if (source[j] == 'e')
        {
            dest[i] = (dest[i] | 0x0e);
        }
        else if (source[j] == 'f')
        {
            dest[i] = (dest[i] | 0x0f);
        }
        else if (source[j] == 'A')
        {
            dest[i] = (dest[i] | 0x0a);
        }
        else if (source[j] == 'B')
        {
            dest[i] = (dest[i] | 0x0b);
        }
        else if (source[j] == 'C')
        {
            dest[i] = (dest[i] | 0x0c);
        }
        else if (source[j] == 'D')
        {
            dest[i] = (dest[i] | 0x0d);
        }
        else if (source[j] == 'E')
        {
            dest[i] = (dest[i] | 0x0e);
        }
        else if (source[j] == 'F')
        {
            dest[i] = (dest[i] | 0x0f);
        }

        j++;
    }
}

// data is a hexstring it corresponds to uint256 type of Solidity
// !!! Attention !!! it must be noted that we supposed that data is uint256, it can be other
// type or even types e.g uint8[6] and bytes32, in that case the two 
// variablse must be concatenated into a uint8_t. Another thing to note
// the data concatenation must be done in the same way in the Smart Contract
// else the signature could not be verified 

void Payload::signedPayload(std::string data, uint8_t ID, std::string privKey, Transaction& transactionObject){

    // string to uint8_t
    uint8_t privateKeyBytes[PRIVATE_KEY_SIZE];
    // HexStrToUchar(privateKeyBytes, &privKey[0], PRIVATE_KEY_SIZE);
   
    
    hexStringToUint8_t(privateKeyBytes, &privKey[0], PRIVATE_KEY_SIZE);

    
    // data is a hexstring 
    int sizeData = data.size()/2;
    uint8_t dataBytes[sizeData];
    // HexStrToUchar(dataBytes, &data[0], data.size());
    
    hexStringToUint8_t(dataBytes, &data[0], sizeData);

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

    // Web3 web3((std::string *)INFURA_HOST, (std::string *)INFURA_PATH);
    // Transaction transaction(&web3, (std::string *)CONTRACT_ADDRESS);

    transactionObject.SetPrivateKey(privateKeyBytes);

    uint8_t signature[SIGNATURE_LENGTH];
    int recid[1] = {0};

    // transactionObject.Sign(hashDigest, signature, recid);
    transactionObject.SignTresor((uint8_t *)hashDigest, signature, recid);

    char out_char_signature[129]; // 64 * 2 + 1
    Util::bytes2hex(signature, out_char_signature, 64);
    std::string out_signature(out_char_signature);    

    std::string signature_r =  out_signature.substr(0, 64);
    std::string signature_s =  out_signature.substr(64, 127);


    r = signature_r;
    s = signature_s;

}

void Payload::Test(){
    r = "IronMan";
    s = "AntMan";
}


