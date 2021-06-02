//
//    Author: Roland Kromes, @ roland.kromes@univ-cotedazur.fr
//    Address: LEAT Laboratory, Université Côte d'Azur
//
//    This API is intended to be used in IoT devices. This library allows to create signed Ethereum
//    Transactions interacting with Smart Contracts
//
//    The digital signature (ECDSA) and keccak hash is done completely local, thus in the device.
//
//    from: is the private key of the Ethereum count that is used for wallet
//
//    Idea of source code comes from: https://github.com/kopanitsa/web3-arduino/blob/master
//    TODO : add curl using Web3js format

#include <iostream>
#include <stdint.h>
#include "Util.h"
#include "Transaction.h"
#include "Payload.h"

#define PRIVATE_KEY_SIZE 32
#define ETH_HOST "http://127.0.0.1:7545"
#define PATH " " // It can be nessecary when using INFURA

#define CONTRACT_ADDRESS "0x8668"
void testFunction(std::string *strValues);
int chhex(char ch);
void HexStrToUchar(unsigned char *dest, const char *source, int bytes_n);
void hexStringToUint8_t(uint8_t *dest, const char *source, int bytes_n);
int main()
{
    Web3 web3((std::string *)ETH_HOST, (std::string *)PATH);
    Transaction transaction(&web3, (std::string *)CONTRACT_ADDRESS);

    // uint8_t toEncode = 1;
    uint8_t res;
    uint32_t mode = 1; // Mode: 1 simple data sending 2: double signature


    // char hexStr[20];
    // Util::BufToCharStr(hexStr, &toEncode, 1);

    // std::cout << std::hex << hexStr << std::endl;

    std::string from = "b37bedd521a67af5cdce434ec9177e2931d82ff92427386d8e8fc8e363d780a4"; //"afc46b7fdfc9eb29d67362a6e4df578adeb2eb610af22ae8b3fcb8b3956c3bf3";

    uint8_t privateKeyBytes[PRIVATE_KEY_SIZE];
    hexStringToUint8_t(privateKeyBytes, &from[0], PRIVATE_KEY_SIZE);

    // HexStrToUchar(privateKeyBytes, &from[0], from.size());

    int i;
    for (i = 0; i < PRIVATE_KEY_SIZE; i++)
    {
        std::cout << (uint32_t)privateKeyBytes[i] << ' ';
    }
    // std::cout << std::endl;
    // std::string uintToStr = (char *) toStr;
    uint32_t nonceVal = 1;
    uint64_t gasPriceVal = 0x4A817C800; //46038239233;
    uint32_t gasLimitVal = 0x6691B7;
    std::string toStr = "0x9d98513642716eA671f2b66cA4b5ad0625308eeE"; //CONTRACT_ADDRESS;
    std::string valueStr = "0x00";                                    //"0x00";
    uint8_t dataStr[100];

    if (mode == 1)
    {
        Payload transactionPayload;
        transaction.SetPrivateKey(privateKeyBytes);

        std::string func = "my_set(uint8[32])"; //"mySet(string)";//"my_set(uint8[32])";
        uint8_t rawdata[32] = {0xdb, 0x8a, 0x04, 0x22, 0x24, 0xc4, 0x4b, 0x05, 0xa9, 0x7e, 0x5f, 0x2a, 0x41, 0x0e, 0xa6, 0x04, 0xd8, 0x18, 0xbb, 0xe9, 0xe6, 0xa5, 0xd2, 0xbe, 0xed, 0x57, 0x78, 0xe7, 0x9e, 0xfd, 0x4a, 0xcf};   
        std::string strData = "Hello";

        char dataString[70];
        Util::BufToCharStr(dataString, rawdata, 32);

        // std::cout << "BufToCharStr " << dataString << std::endl;

        std::string p = transaction.SetupContractData(&func, rawdata);

        // std::cout << "Contract " << p << std::endl;

        std::string result = transaction.createTransaction(nonceVal, gasPriceVal, gasLimitVal, &toStr, &valueStr, &p);
        std::cout << "To send : " << result << std::endl;

    }
    else if (mode == 2)
    {

        std::string func = "setMyData(uint256,uint8,uint256,uint256)"; // "my_set(uint8[32])"; //"get(uint256)";

        Payload transactionPayload;
        uint8_t ID = 0x97;
        std::string payloadData = "db8a042224c44b05a97e5f2a410ea604d818bbe9e6a5d2beed5778e79efd4acf";
  
        string secondPrivateKey = "2a53a7438211849cb95cd4ffe182e01711c34dd525707ab1a0a30fff0b18605b";

        transactionPayload.signedPayload(payloadData, ID, secondPrivateKey, transaction);
        transaction.SetPrivateKey(privateKeyBytes);

        std::cout << "########## Transaction Payload Test" << std::endl;
        std::cout << " Signature r = " << transactionPayload.r << std::endl;
        std::cout << " Signature s = " << transactionPayload.s << std::endl;

        uint32_t dataParams = 123;

        // std::string func2 = "setMyData(string)";
        // std::string payload2 = "Hello ";
        // std::string p3 = transaction.SetupContractData(&func2, payload2);
        // std::cout << "###### Contract " << p3 << std::endl;

        std::string p2 = transaction.SetupContractData(&func, payloadData,&ID,transactionPayload.r, transactionPayload.s);

        // std::cout << "Contract " << p2 << std::endl;

        std::string result = transaction.createTransaction(nonceVal, gasPriceVal, gasLimitVal, &toStr, &valueStr, &p2);
        std::cout << "To send : " << result << std::endl;

    }


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


void hexStringToUint8_t(uint8_t *dest, const char *source, int bytes_n)
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