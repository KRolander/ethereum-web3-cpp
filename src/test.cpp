//
//    Author: Roland Kromes, @ roland.kromes@univ-cotedazur.fr
//    Address: LEAT Laboratory, Université Côte d'Azur
//
//    This API is intended to be used in IoT devices. This library allows to create signed Ethereum
//    Transactions interacting with Smart Contracts
//    The digital signature and keccak hash is done completely local, thus in the device.
//
//    from: is the private key of the Ethereum count that is used for wallet  
// 
//    Idea of source code comes from: https://github.com/kopanitsa/web3-arduino/blob/master 
//    TODO : add curl using Web3js format



#include <iostream>
#include <stdint.h>
#include "Util.h"
#include "Transaction.h"
#define PRIVATE_KEY_SIZE 32
#define INFURA_HOST "rinkeby.infura.io"
#define INFURA_PATH "/<YOUR_INFURA_ID>"

#define CONTRACT_ADDRESS "0x8668"
void testFunction(std::string *strValues);
int chhex(char ch);
void HexStrToUchar(unsigned char *dest, const char *source, int bytes_n);
int main()
{
    Web3 web3((std::string *)INFURA_HOST, (std::string *)INFURA_PATH);
    Transaction transaction(&web3, (std::string *)CONTRACT_ADDRESS);

    uint8_t toEncode = 1;
    uint8_t res;
    uint32_t resLenght;

    resLenght = Util::RlpEncodeItem(&res, &toEncode, 1);

    char hexStr[20];
    Util::BufToCharStr(hexStr, &toEncode, 1);

    std::cout << std::hex << hexStr << std::endl;

    // uint8_t toStr[] = CONTRACT_ADDRESS;
    std::string from = "afc46b7fdfc9eb29d67362a6e4df578adeb2eb610af22ae8b3fcb8b3956c3bf3";
    // Set PrivateKey
    // std::vector<uint8_t> privetKeyVector = Util::ConvertStringToVector(&from);
    // uint8_t * privetKeyArray = privetKeyVector.data();
    // contract.SetPrivateKey(privetKeyArray);

    uint8_t privateKeyBytes[PRIVATE_KEY_SIZE];
    HexStrToUchar(privateKeyBytes, &from[0], from.size());

    transaction.SetPrivateKey(privateKeyBytes);

    int i;
    for (i = 0; i < PRIVATE_KEY_SIZE; i++)
    {
        std::cout << (uint32_t)privateKeyBytes[i] << ' ';
    }
    std::cout << std::endl;

    // std::string uintToStr = (char *) toStr;
    uint32_t nonceVal = 7;
    uint64_t gasPriceVal = 46038239233;
    uint32_t gasLimitVal = 6721975;
    std::string toStr = "0xE168ACB9CE197c378c2e0E211b5A79D685922b77"; //CONTRACT_ADDRESS;
    std::string valueStr = "0x00";                      //"0x00";
    uint8_t dataStr[100];
    std::string func = "my_set(uint8[32])"; //"get(uint256)";

    uint32_t dataParams = 123;
    char dataString[70]; 
    uint8_t rawdata[32] = {0xdb, 0x8a, 0x04, 0x22, 0x24, 0xc4, 0x4b, 0x05, 0xa9, 0x7e, 0x5f, 0x2a, 0x41, 0x0e, 0xa6, 0x04, 0xd8, 0x18, 0xbb, 0xe9, 0xe6, 0xa5, 0xd2, 0xbe, 0xed, 0x57, 0x78, 0xe7, 0x9e, 0xfd, 0x4a, 0xcf};
    
   
    Util::BufToCharStr(dataString, rawdata, 32);
 
    std::cout << "BufToCharStr " << dataString << std::endl;

    // char *cstr = new char[dataString.length() + 1];
    // strcpy(cstr, dataString.c_str());
    // uint8_t dest[32]; // = new uint8_t[dataString.length() + 1];
   
   
   
    // HexStrToUchar(dest, &dataString[0], 32);
    
    // int k;
    // for (k = 0; k<32; k++)
    // {
    //     std::cout << (uint32_t)dest[k] << ' ';
    // }
    // std::cout << std::endl;
    uint8_t uintData[2] = {0xdb, 0x8a};

    std::string p2 = transaction.SetupContractData(&func, rawdata);

    std::cout << "Contract " << p2 << std::endl;

    std::string p = "0x6d4ce63c";

    std::string result = transaction.SendTransaction(nonceVal, gasPriceVal, gasLimitVal, &toStr, &valueStr, &p2);

    std::cout << "To send : " << result << std::endl;

    // testFunction(&valueStr);

    // std::vector<uint8_t> encoded = contract.RlpEncode_v2(nonceVal, gasPriceVal, gasLimitVal, &toStr, &valueStr, &p);

    //std::cout << encoded[0] << std::endl;

    // int i;
    // for(i=0; i<encoded.size(); i++)
    // {
    //     std::cout << (uint32_t) encoded[i] << std::endl;
    // }
    // std::cout << std::hex << uintToStr << std::endl;

    return 0;
}
void testFunction(std::string *strValues)
{
    std::cout << strValues[0] << std::endl;
}

void HexStrToUchar(unsigned char *dest, const char *source, int bytes_n)
{
    for (bytes_n--; bytes_n >= 0; bytes_n--)
        dest[bytes_n] = 16 * chhex(source[bytes_n * 2]) + chhex(source[bytes_n * 2 + 1]);
}

int chhex(char ch)
{
    if (isdigit(ch))
        return ch - '0';
    if (tolower(ch) >= 'a' && tolower(ch) <= 'f')
        return ch - 'a' + 10;
    return -1;
}
