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

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <sstream>
#include <iterator>

#include "Transaction.h"
#include "Web3.h"
#include "Util.h"
#include <vector>
#include <stdarg.h>

// #include "../crypto/sha3.h"
// #include "../XKCP/Keccak-readable-and-compact.c"

#include "secp256k1/include/secp256k1_recovery.h"
#include "secp256k1/src/secp256k1_c.h"
#include "secp256k1/src/module/recovery/main_impl.h"
/////////

#define SIGNATURE_LENGTH 64
#define SHA3_256_DIGEST_LENGTH 32

static secp256k1_context *ctx;

vector<uint8_t> encodedVector;

/**
 * Public functions
 * */

Transaction::Transaction(Web3 *_web3, const string *address)
{
    web3 = _web3;
    contractAddress = address;
    options.gas = 0;
    strcpy(options.from, "0x4409e4cB0f6a0A4C3a840fB2B1Ff6800d6032B92");
    strcpy(options.to, "");
    strcpy(options.gasPrice, "0");

    // ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
}

void Transaction::SetPrivateKey(const uint8_t *key)
{
    privateKey = key;
}

string Transaction::SetupContractData(const string *func, ...)
{
    string ret = "";

    // address
    string contractBytes = GenerateContractBytes(func);
    ret = contractBytes;

    size_t paramCount = 0;
    vector<string> params;
    char *p;
    char tmp[func->size()];
    memset(tmp, 0, sizeof(tmp));
    strcpy(tmp, func->c_str());
    strtok(tmp, "(");
    p = strtok(NULL, "(");
    p = strtok(p, ")");
    p = strtok(p, ",");
    if (p != NULL)
    {
        // std::cout << "strtok => " << p << std::endl;
        params.push_back(string(p));
        paramCount++;
    }
    while (p != 0)
    {
        p = strtok(0, ",");
        if (p != 0)
        {
            // std::cout << "strtok 1 => " << p << std::endl;
            params.push_back(string(p));
            paramCount++;
        }
    }
    // std::cout << "Here : " << paramCount << std::endl;

    va_list args;
    va_start(args, paramCount);
    for (int i = 0; i < paramCount; ++i)
    {
        //     std::cout << "params[0].c_str() " << params[0].c_str() << std::endl;
        //     std::cout << "params[1].c_str() " << params[1].c_str() << std::endl;
        //     std::cout << "params[2].c_str() " << params[2].c_str() << std::endl;
        //     std::cout << "params[3].c_str() " << params[3].c_str() << std::endl;

        // if (strncmp(params[0].c_str(), "uint256", 7 /*sizeof("bytes")*/) == 0)
        // {
        //     std::cout << "======= > Here is okay " << std::endl;
        // }
        // if (strncmp(params[1].c_str(), "bytes", 5 /*sizeof("bytes")*/) == 0)
        // {
        //     std::cout << "======= > Here is okay 2" << std::endl;
        // }

        //  std::cout << "Params " << params[i].c_str() << " i == " << i << std::endl;
        if (strncmp(params[i].c_str(), "uint", 4 /*sizeof("uint")*/) == 0)
        {
            //  std::cout << " ############## Here " << std::endl;
            if ((strncmp(params[i].c_str(), "uint8[", 6 /*sizeof("uint")*/) == 0) && (strncmp(params[i].c_str(), "uint8[]", 7 /*sizeof("uint")*/) != 0))
            {
                std::cout << "uint8[ is detected" << std::endl;

                char *p2;
                char tmp2[params[i].size()];
                memset(tmp2, 0, sizeof(tmp2));
                strcpy(tmp2, params[i].c_str());
                strtok(tmp2, "[");
                // p2 = strtok(NULL, "(");
                // p2 = strtok(p2, ")");
                // p2 = strtok(p2, ",");
                p2 = strtok(NULL, "]");

                int numbOfTabElements;
                numbOfTabElements = std::atoi(p2);

                // Get data (uint8_t[]) array
                uint8_t *data = va_arg(args, uint8_t *);

                string zeros = "00000000000000000000000000000000000000000000000000000000000000";

                //string output;

                char charData[numbOfTabElements * 2 + 1];
                Util::bytes2hex(data, charData, numbOfTabElements);
                std::string tmpData(charData);

                int k;
                for (k = 0; k < numbOfTabElements * 2 - 1; k += 2)
                    ret = ret + zeros + tmpData[k] + tmpData[k + 1];

                // ret = ret + output;
                // std::cout << " Overall data : " << ret << std::endl;
            }
            else if ((strncmp(params[i].c_str(), "uint8", 5 /*sizeof("uint")*/) == 0) && (strncmp(params[i].c_str(), "uint8[", 6 /*sizeof("uint")*/) != 0) && (strncmp(params[i].c_str(), "uint8[]", 7 /*sizeof("uint")*/) != 0))
            {
                std::cout << "uint8 is detected" << std::endl;

                // string output = GenerateBytesForUint(va_arg(args, uint32_t));
                uint8_t *data = va_arg(args, uint8_t *);
                string zeros = "00000000000000000000000000000000000000000000000000000000000000";

                // std::cout << "Data " << (uint32_t) data << std::endl;

                char charData[3];
                Util::bytes2hex(data, charData, 2);
                std::string tmpData(charData);

                ret = ret + zeros + tmpData[0] + tmpData[1];
                // ret = ret + zeros + '7' + '7';
            }
            else if (strncmp(params[i].c_str(), "uint8[]", 7 /*sizeof("uint")*/) == 0)
            {

                std::cout << "###### Pleasse Precise length of the input array" << std::endl;
            }
            else if ((strncmp(params[i].c_str(), "uint16[", 7 /*sizeof("uint")*/) == 0) && (strncmp(params[i].c_str(), "uint16[]", 8 /*sizeof("uint")*/) != 0))
            {

                // std::cout << "uint16[ is detected" << std::endl;

                char *p2;
                char tmp2[params[i].size()];
                memset(tmp2, 0, sizeof(tmp2));
                strcpy(tmp2, params[i].c_str());
                strtok(tmp2, "[");
                // p2 = strtok(NULL, "(");
                // p2 = strtok(p2, ")");
                // p2 = strtok(p2, ",");
                p2 = strtok(NULL, "]");

                int numbOfTabElements;
                numbOfTabElements = std::atoi(p2);

                // Get data (uint8_t[]) array
                uint16_t *data = va_arg(args, uint16_t *);

                std::string zeros = "000000000000000000000000000000000000000000000000000000000000";

                char charData[numbOfTabElements * 4 + 1];
                Util::doubleBytes2hex(data, charData, numbOfTabElements * 4);
                std::string tmpData(charData);

                int k;
                for (k = 0; k < numbOfTabElements * 4 - 1; k += 4)
                    ret = ret + zeros + tmpData[k] + tmpData[k + 1] + tmpData[k + 2] + tmpData[k + 3];
            }
            else if ((strncmp(params[i].c_str(), "uint16", 6 /*sizeof("uint")*/) == 0) && (strncmp(params[i].c_str(), "uint16[", 7 /*sizeof("uint")*/) != 0) && (strncmp(params[i].c_str(), "uint16[]", 8 /*sizeof("uint")*/) != 0))
            {
                uint16_t *data = va_arg(args, uint16_t *);

                std::string zeros = "000000000000000000000000000000000000000000000000000000000000";
                char charData[5];
                Util::doubleBytes2hex(data, charData, 4);
                std::string tmpData(charData);

                ret = ret + zeros + tmpData[0] + tmpData[1] + tmpData[2] + tmpData[3];
            }
            else if (strncmp(params[i].c_str(), "uint16[]", 8 /*sizeof("uint")*/) == 0)
            {
                // std::cout << "###### Pleasse Precise length of the input array" << std::endl;
            }
            else if ((strncmp(params[i].c_str(), "uint32[", 7 /*sizeof("uint")*/) == 0) && (strncmp(params[i].c_str(), "uint32[]", 8 /*sizeof("uint")*/) != 0))
            {
                // std::cout << "uint32[ is detected" << std::endl;

                char *p2;
                char tmp2[params[i].size()];
                memset(tmp2, 0, sizeof(tmp2));
                strcpy(tmp2, params[i].c_str());
                strtok(tmp2, "[");
                // p2 = strtok(NULL, "(");
                // p2 = strtok(p2, ")");
                // p2 = strtok(p2, ",");
                p2 = strtok(NULL, "]");

                int numbOfTabElements;
                numbOfTabElements = std::atoi(p2);

                // Get data (uint8_t[]) array
                uint32_t *data = va_arg(args, uint32_t *);

                std::string zeros = "00000000000000000000000000000000000000000000000000000000";

                char charData[numbOfTabElements * 8 + 1];
                Util::quarteBytes2hex(data, charData, numbOfTabElements * 8);
                std::string tmpData(charData);

                int k;
                for (k = 0; k < numbOfTabElements * 8 - 1; k += 8)
                    ret = ret + zeros + tmpData[k] + tmpData[k + 1] + tmpData[k + 2] + tmpData[k + 3] + tmpData[k + 4] + tmpData[k + 5] + tmpData[k + 6] + tmpData[k + 7];
            }
            else if ((strncmp(params[i].c_str(), "uint32", 6 /*sizeof("uint")*/) == 0) && (strncmp(params[i].c_str(), "uint32[", 7 /*sizeof("uint")*/) != 0) && (strncmp(params[i].c_str(), "uint32[]", 8 /*sizeof("uint")*/) != 0))
            {
                uint32_t *data = va_arg(args, uint32_t *);

                std::string zeros = "00000000000000000000000000000000000000000000000000000000";
                char charData[9];
                Util::quarteBytes2hex(data, charData, 8);
                std::string tmpData(charData);

                ret = ret + zeros + tmpData[0] + tmpData[1] + tmpData[2] + tmpData[3] + tmpData[4] + tmpData[5] + tmpData[6] + tmpData[7];
            }
            else if (strncmp(params[i].c_str(), "uint32[]", 8 /*sizeof("uint")*/) == 0)
            {
                // std::cout << "###### Pleasse Precise length of the input array" << std::endl;
            }
            // uint256 is already a hex string so nothing to do
            else if (strncmp(params[i].c_str(), "uint256", 7 /*sizeof("uint")*/) == 0)
            {
                std::cout << "uint256 is detected" << std::endl;
                ret = ret + va_arg(args, string);
                std::cout << " => => => => " << ret << std::endl;
            }
            else
            {
                // std::cout << "Please precise the exact type" << std::endl;
            }
        }
        else if (strncmp(params[i].c_str(), "string", sizeof("string")) == 0)
        {
            // std::cout << "String is detected" << std::endl;
            string output = GenerateBytesForString(va_arg(args, string *));
            ret = ret + string(output);
        }
        else if (strncmp(params[i].c_str(), "bytes", 5 /*sizeof("bytes")*/) == 0)
        {
            // std::cout << " ########### Bytes32 is detected" << std::endl;

            const char *tmp = va_arg(args, char *);

            // remove 0x

            char *ptr = strtok((char *)tmp, "x");
            if (strlen(ptr) != 1)
            {
                ptr = (char *)tmp;
            }
            else
            {
                ptr = strtok(NULL, "x");
            }

            // char charOutputTest[63] = {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1','1', '1', '1', '1', '1', '1'};
            char charOutput[65] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

            int lenght = strlen(ptr);

            if (lenght == 64)
            {
                ret = ret + string(ptr);
            }
            else
            {
                int k;
                int diff = 64 - lenght;
                for (k = 0; k < 64 - diff; k++)
                {
                    charOutput[k] = ptr[k];
                }
                // printf("Here is my output %s\n", charOutput);
                ret = ret + string(charOutput);
            }
            // std::cout << " => => => => " << ret << std::endl;
        }
        else
        {
            std::cout << " Problem HERE " << std::endl;
        }
    }
    va_end(args);
    // std::cout << "Here" << std::endl;

    return ret;
}

string Transaction::Call(const string *param)
{
    const string from = string(options.from);
    const long gasPrice = strtol(options.gasPrice, nullptr, 10);
    const string value = "";

    string result = web3->EthCall(&from, contractAddress, options.gas, gasPrice, &value, param);
    return result;
}

string Transaction::createTransaction(uint32_t nonceVal, uint64_t gasPriceVal, uint32_t gasLimitVal,
                                      string *toStr, string *valueStr, string *dataStr)
{

    uint8_t signature[SIGNATURE_LENGTH];
    memset(signature, 0, SIGNATURE_LENGTH);
    int recid[1] = {0};
    GenerateSignature(signature, recid, nonceVal, gasPriceVal, gasLimitVal,
                      toStr, valueStr, dataStr);

    vector<uint8_t> param = RlpEncodeForRawTransaction(nonceVal, gasPriceVal, gasLimitVal,
                                                       toStr, valueStr, dataStr,
                                                       signature, recid[0]);
    // string paramStr = Util::VectorToString(param);

    char param_c[param.size() * 2 + 1]; // 32 * 2 + 1
    Util::bytes2hex(&param[0], param_c, param.size());

    std::string paramStr = "0x";
    std::string param_hex(param_c);

    paramStr = paramStr + param_hex;

    // std::cout << "Param to send : " << paramStr << std::endl;

    encodedVector.clear();
    return web3->EthSendSignedTransaction(&paramStr, param.size());
}

/**
 * Private functions
 * */

std::string UcharToHexStr(unsigned char *data, int len) //bytes to string
{

    //the following is better: IT FILLS WITH 0 !!!!
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < len; ++i)
        ss << std::setw(2) << static_cast<unsigned>(data[i]);
    return ss.str();
}

void Transaction::hexStringToUint8_t(uint8_t *dest, const char *source, int bytes_n)
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

void Transaction::HexStrToUchar(unsigned char *dest, const char *source, int bytes_n)
{
    for (bytes_n--; bytes_n >= 0; bytes_n--)
        dest[bytes_n] = 16 * chhex(source[bytes_n * 2]) + chhex(source[bytes_n * 2 + 1]);
}

int Transaction::chhex(char ch)
{
    if (isdigit(ch))
        return ch - '0';
    if (tolower(ch) >= 'a' && tolower(ch) <= 'f')
        return ch - 'a' + 10;
    return -1;
}

void Transaction::GenerateSignature(uint8_t *signature, int *recid, uint32_t nonceVal, uint64_t gasPriceVal, uint32_t gasLimitVal,
                                    string *toStr, string *valueStr, string *dataStr)
{

    vector<uint8_t> encoded = RlpEncode(nonceVal, gasPriceVal, gasLimitVal, toStr, valueStr, dataStr);

    char t_c[encoded.size() * 2 + 1]; // 32 * 2 + 1
    Util::bytes2hex(&encoded[0], t_c, encoded.size());

    std::string t = "0x";
    std::string t_hex(t_c);

    t = t + t_hex;

    // std::cout << " TODO Sha3 in GenerateSignature" << std::endl;

    t.erase(0, 2); // remove '0x'

    // std::cout << " t : " << t << " size " << t.size() << std::endl;

    int toHashSize = t.size() / 2;
    uint8_t *toHash = new uint8_t[toHashSize];

    hexStringToUint8_t(toHash, &t[0], toHashSize);

    uint8_t digest[SHA3_256_DIGEST_LENGTH];
    // FIPS202_SHA3_256(toHash, toHashSize, digest);

    keccak_256(toHash, toHashSize, digest);

    // Sign((uint8_t *)digest, signature, recid);
    SignTresor((uint8_t *)digest, signature, recid);
    int i;
    // std::cout << "Signature" << std::endl;
    // std::cout << "r : " << std::endl;
    // for (i = 0; i < 32; i++)
    // {
    //     std::cout << (uint32_t)signature[i] << ' ';
    // }
    // std::cout << std::endl;
    // std::cout << "s : " << std::endl;
    // for (i = 32; i < 64; i++)
    // {
    //     std::cout << (uint32_t)signature[i] << ' ';
    // }
    // std::cout << std::endl;
#if 0
    printf("\nhash_input : %s\n", tmp);
    printf("\nhash_output: %s\n", hashedStr);
    printf("\nhash--------\n ");
    for (int i = 0; i<32; i++) {
        printf("%02x ", tmp[i]);
    }
#endif
}

string Transaction::GenerateContractBytes(const string *func)
{
    string in = "0x";
    char intmp[8];
    memset(intmp, 0, 8);
    int i = 0;
    for (int i = 0; i < 128; i++)
    {
        char c = (*func)[i];
        if (c == '\0')
        {
            break;
        }
        sprintf(intmp, "%x", c);
        in = in + intmp;
    }
    // TODO :

    in.erase(0, 2);

    int toHashSize = in.size() / 2;
    uint8_t *toHash = new uint8_t[toHashSize];

    hexStringToUint8_t(toHash, &in[0], toHashSize);
    uint8_t digest[SHA3_256_DIGEST_LENGTH];
    // FIPS202_SHA3_256(toHash, toHashSize, digest);

    keccak_256(toHash, toHashSize, digest);

    // std::cout << "SC Keccak : " << std::endl;

    // for (i = 0; i < 32; i++)
    // {
    //     std::cout << (uint32_t)digest[i] << ' ';
    // }
    // std::cout << std::endl;

    // // string out = web3->Web3Sha3(&in);
    // std::cout << " TODO web3->Web3Sha3 " << std::endl;
    // std::cout << "Commande to hash : " << in << std::endl;

    // TODO : ADD
    // Digest to hex
    vector<uint8_t> hashVector;
    for (i = 0; i < 32; i++)
    {
        hashVector.push_back(digest[i]);
    }

    // std::string out = Util::VectorToString(hashVector);

    // std::cout << "String Hash " << out << std::endl;

    char out_c[65]; // 32 * 2 + 1
    Util::bytes2hex(digest, out_c, 32);

    std::string out_alternative = "0x";
    std::string out_hash(out_c);

    out_alternative = out_alternative + out_hash;
    // std::cout << "String Hash alternative " << out_alternative << std::endl;

    // std::string out = "0x6d4ce63caa65600744ac797760560da39ebd16e8240936b51f53368ef9e0e01f";

    // out.resize(10);
    out_alternative.resize(10);

    // return out;
    return out_alternative;
}

string Transaction::GenerateBytesForString(const string *value)
{

    std::string strValue = value[0];
    uint32_t n = strValue.length();

    char charValue[n + 1];
    strcpy(charValue, strValue.c_str());

    char dummy[2];

    std::string tmpOut;

    int i;
    for (i = 0; i < n; i++)
    {
        int digits = sprintf(dummy, "%x", (uint32_t)charValue[i]); // Get the utf-8 hex value of the string
        tmpOut = tmpOut + std::string(dummy);
    }

    std::string zerosForNumOfChar = "00000000000000000000000000000000000000000000000000000000";
    char charData[9];

    Util::quarteBytes2hex(&n, charData, 8);
    std::string numOfChar(charData);

    std::string prefix = "0000000000000000000000000000000000000000000000000000000000000020"; // string type prefix
    string zeros = "";

    for (i = 0; i < (32 - n); i++)
        zeros = zeros + "00"; // zero pending to right

    return prefix + numOfChar[0] + numOfChar[1] + numOfChar[2] + numOfChar[3] + numOfChar[4] + numOfChar[5] + numOfChar[6] + numOfChar[7] + tmpOut + zeros;
}

void printArrays(int sizeOfArray, uint8_t * dataArray)
{
    int k;
    std::cout << "__________________________________________" << std::endl;
    for(k=0; k<sizeOfArray; k++)
        std::cout << std::hex << (uint32_t) dataArray[k] << " ";
    std::cout << std::endl; 
    std::cout << "__________________________________________" << std::endl;
}

vector<uint8_t> Transaction::RlpEncode(
    uint32_t nonceVal, uint64_t gasPriceVal, uint32_t gasLimitVal,
    string *toStr, string *valueStr, string *dataStr)
{

    vector<uint8_t> nonce = Util::ConvertNumberToVector(nonceVal);

    uint64_t gPv = 46038239233; // 20000000000;
    vector<uint8_t> gasPrice = Util::ConvertNumberToVector64(gasPriceVal);
    // vector<uint8_t> gasPrice = Util::ConvertNumberToVector(gasPriceVal);

    vector<uint8_t> gasLimit = Util::ConvertNumberToVector(gasLimitVal);
    vector<uint8_t> to = Util::ConvertStringToVector(toStr);

    vector<uint8_t> value = Util::ConvertStringToVector(valueStr);

    std::vector<uint8_t> data = Util::ConvertStringToVector(dataStr);

   
///////////////////////////////////////////////////////////////////////////////
// Test with vectors

    // std::cout << "Conversions to Array" << std::endl;
    // std::cout << std::endl;

    // uint8_t nonceValArray[4];
    // int sizeArraygasLimit = Util::convertUint32ToUint8Array(nonceVal, nonceValArray);
    // printArrays(sizeArraygasLimit, nonceValArray);

    // uint8_t gasLimitArray[4];
    // int sizeArraynonceVal = Util::convertUint32ToUint8Array(gasLimitVal, gasLimitArray);
    // printArrays(sizeArraynonceVal, gasLimitArray);

    // uint8_t gasPriceArray[8];
    // int sizeArrayGasPrice = Util::convertUint64ToUint8Array(gasPriceVal, gasPriceArray);
    // printArrays(sizeArrayGasPrice, gasPriceArray);

    // std::cout << "toStr: " <<  toStr[0] << std::endl;

    // int sizeArraytoStr = (toStr->size() / 2) - 1;
    // uint8_t toStrArray[sizeArraytoStr];
    // Util::ConvertStringToArray(toStr, toStrArray, sizeArraytoStr);
    // printArrays(sizeArraytoStr, toStrArray);

    // std::cout << "valueStr: " <<  valueStr[0] << std::endl;
     
    // int sizeArrayvalueStr = (valueStr->size() / 2) - 1;
    // uint8_t valueStrArray[sizeArrayvalueStr];
    // Util::ConvertStringToArray(valueStr, valueStrArray, sizeArrayvalueStr);
    // printArrays(sizeArrayvalueStr, valueStrArray);
    
    // // std::cout << "Data: " <<  dataStr[0] << std::endl;
    // int sizeArraydataStr = (dataStr->size() / 2) - 1;
    // uint8_t dataStrArray[sizeArraydataStr];
    // Util::ConvertStringToArray(dataStr, dataStrArray, sizeArraydataStr);
    // printArrays(sizeArraydataStr, dataStrArray);
    
///////////////////////////////////////////////////////////////////////////////   


    // int k;
    // for(k=0; k< gasPrice.size(); k++)
    //     std::cout << std::hex << (uint32_t) gasPrice[k] << " ";
    // std::cout << std::endl;  

    // int dataArraySize = (dataStr->size() / 2) - 1;
    // uint8_t dataArray[dataArraySize];
    // Util::ConvertStringToArray(dataStr, dataArray, dataArraySize);
    
    // std::cout << "__________________________________________" << std::endl;



    // for(k=0; k<sizeArrayGasPrice; k++)
    //     std::cout << std::hex << (uint32_t) gasPriceArray[k] << " ";
    // std::cout << std::endl;    


    vector<uint8_t> outputNonce = Util::RlpEncodeItemWithVector(nonce);
    vector<uint8_t> outputGasPrice = Util::RlpEncodeItemWithVector(gasPrice);
    vector<uint8_t> outputGasLimit = Util::RlpEncodeItemWithVector(gasLimit);
    vector<uint8_t> outputTo = Util::RlpEncodeItemWithVector(to);
    vector<uint8_t> outputValue = Util::RlpEncodeItemWithVector(value);

    vector<uint8_t> outputData = Util::RlpEncodeItemWithVector(data);

    
    encodedVector.insert(encodedVector.end(), outputNonce.begin(), outputNonce.end());
    encodedVector.insert(encodedVector.end(), outputGasPrice.begin(), outputGasPrice.end());
    encodedVector.insert(encodedVector.end(), outputGasLimit.begin(), outputGasLimit.end());
    encodedVector.insert(encodedVector.end(), outputTo.begin(), outputTo.end());
    encodedVector.insert(encodedVector.end(), outputValue.begin(), outputValue.end());
    encodedVector.insert(encodedVector.end(), outputData.begin(), outputData.end());
    
    vector<uint8_t> encoded = Util::RlpEncodeWholeHeaderWithVector(
        outputNonce.size() +
        outputGasPrice.size() +
        outputGasLimit.size() +
        outputTo.size() +
        outputValue.size() +
        outputData.size());
    // prefix.size());

    encoded.insert(encoded.end(), encodedVector.begin(), encodedVector.end());
    // encoded.insert(encoded.end(), outputNonce.begin(), outputNonce.end());
    // encoded.insert(encoded.end(), outputGasPrice.begin(), outputGasPrice.end());
    // encoded.insert(encoded.end(), outputGasLimit.begin(), outputGasLimit.end());
    // encoded.insert(encoded.end(), outputTo.begin(), outputTo.end());
    // encoded.insert(encoded.end(), outputValue.begin(), outputValue.end());
    // encoded.insert(encoded.end(), outputData.begin(), outputData.end());

    return encoded;
}

vector<uint8_t> Transaction::RlpEncode_v2(
    uint32_t nonceVal, uint32_t gasPriceVal, uint32_t gasLimitVal,
    string *toStr, string *valueStr, string *dataStr)
{
    vector<uint8_t> nonce = Util::ConvertNumberToVector(nonceVal);
    vector<uint8_t> gasPrice = Util::ConvertNumberToVector(gasPriceVal);
    vector<uint8_t> gasLimit = Util::ConvertNumberToVector(gasLimitVal);
    vector<uint8_t> to = Util::ConvertStringToVector(toStr);
    vector<uint8_t> value = Util::ConvertStringToVector(valueStr);
    vector<uint8_t> data = Util::ConvertStringToVector(dataStr);

    vector<uint8_t> outputNonce = Util::RlpEncodeItemWithVector(nonce);
    vector<uint8_t> outputGasPrice = Util::RlpEncodeItemWithVector(gasPrice);
    vector<uint8_t> outputGasLimit = Util::RlpEncodeItemWithVector(gasLimit);
    vector<uint8_t> outputTo = Util::RlpEncodeItemWithVector(to);
    vector<uint8_t> outputValue = Util::RlpEncodeItemWithVector(value);
    vector<uint8_t> outputData = Util::RlpEncodeItemWithVector(data);

    vector<uint8_t> encoded = Util::RlpEncodeWholeHeaderWithVector(
        outputNonce.size() +
        outputGasPrice.size() +
        outputGasLimit.size() +
        outputTo.size() +
        outputValue.size() +
        outputData.size());

    encoded.insert(encoded.end(), outputNonce.begin(), outputNonce.end());
    encoded.insert(encoded.end(), outputGasPrice.begin(), outputGasPrice.end());
    encoded.insert(encoded.end(), outputGasLimit.begin(), outputGasLimit.end());
    encoded.insert(encoded.end(), outputTo.begin(), outputTo.end());
    encoded.insert(encoded.end(), outputValue.begin(), outputValue.end());
    encoded.insert(encoded.end(), outputData.begin(), outputData.end());

    return encoded;
}

void Transaction::Sign(uint8_t *hash, uint8_t *sig, int *recid)
{
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

    secp256k1_nonce_function noncefn = secp256k1_nonce_function_rfc6979;
    void *data_ = NULL;

    secp256k1_ecdsa_recoverable_signature signature;
    memset(&signature, 0, sizeof(signature));
    int i;

    if (secp256k1_ecdsa_sign_recoverable(ctx, &signature, hash, privateKey, noncefn, data_) == 0)
    {
        return;
    }

    secp256k1_ecdsa_recoverable_signature_serialize_compact(ctx, &sig[0], &recid[0], &signature);
}

void Transaction::SignTresor(uint8_t *hash, uint8_t *sig, int *recid)
{
    const ecdsa_curve *curve = &secp256k1;
    uint8_t py;

    int ok = ecdsa_sign_digest(curve, privateKey, hash, sig, &py, NULL);

    recid[0] = py;
}

vector<uint8_t> Transaction::RlpEncodeForRawTransaction(
    uint32_t nonceVal, uint64_t gasPriceVal, uint32_t gasLimitVal,
    string *toStr, string *valueStr, string *dataStr, uint8_t *sig, uint8_t recid)
{

    vector<uint8_t> signature;
    for (int i = 0; i < SIGNATURE_LENGTH; i++)
    {
        signature.push_back(sig[i]);
    }
    // TODO : Vectors
    // vector<uint8_t> nonce = Util::ConvertNumberToVector(nonceVal);

    // vector<uint8_t> gasPrice = Util::ConvertNumberToVector64(gasPriceVal);

    // vector<uint8_t> gasLimit = Util::ConvertNumberToVector(gasLimitVal);
    // vector<uint8_t> to = Util::ConvertStringToVector(toStr);
    // vector<uint8_t> value = Util::ConvertStringToVector(valueStr);
    // vector<uint8_t> data = Util::ConvertStringToVector(dataStr);

    // vector<uint8_t> outputNonce = Util::RlpEncodeItemWithVector(nonce);
    // vector<uint8_t> outputGasPrice = Util::RlpEncodeItemWithVector(gasPrice);
    // vector<uint8_t> outputGasLimit = Util::RlpEncodeItemWithVector(gasLimit);
    // vector<uint8_t> outputTo = Util::RlpEncodeItemWithVector(to);
    // vector<uint8_t> outputValue = Util::RlpEncodeItemWithVector(value);
    // vector<uint8_t> outputData = Util::RlpEncodeItemWithVector(data);

    vector<uint8_t> R;
    R.insert(R.end(), signature.begin(), signature.begin() + (SIGNATURE_LENGTH / 2));
    vector<uint8_t> S;
    S.insert(S.end(), signature.begin() + (SIGNATURE_LENGTH / 2), signature.end());
    vector<uint8_t> V;
    V.push_back((uint8_t)(recid + 27)); // 27 is a magic number for Ethereum spec
    vector<uint8_t> outputR = Util::RlpEncodeItemWithVector(R);
    vector<uint8_t> outputS = Util::RlpEncodeItemWithVector(S);
    vector<uint8_t> outputV = Util::RlpEncodeItemWithVector(V);

    // vector<uint8_t> encoded = Util::RlpEncodeWholeHeaderWithVector(
    //     outputNonce.size() +
    //     outputGasPrice.size() +
    //     outputGasLimit.size() +
    //     outputTo.size() +
    //     outputValue.size() +
    //     outputData.size() +
    //     outputR.size() +
    //     outputS.size() +
    //     outputV.size());

    // encoded.insert(encoded.end(), outputNonce.begin(), outputNonce.end());
    // encoded.insert(encoded.end(), outputGasPrice.begin(), outputGasPrice.end());
    // encoded.insert(encoded.end(), outputGasLimit.begin(), outputGasLimit.end());
    // encoded.insert(encoded.end(), outputTo.begin(), outputTo.end());
    // encoded.insert(encoded.end(), outputValue.begin(), outputValue.end());
    // encoded.insert(encoded.end(), outputData.begin(), outputData.end());
    // encoded.insert(encoded.end(), outputV.begin(), outputV.end());
    // encoded.insert(encoded.end(), outputR.begin(), outputR.end());
    // encoded.insert(encoded.end(), outputS.begin(), outputS.end());

    vector<uint8_t> encoded = Util::RlpEncodeWholeHeaderWithVector(
        outputR.size() +
        outputS.size() +
        outputV.size() +
        encodedVector.size());

    encoded.insert(encoded.end(),encodedVector.begin(),encodedVector.end());
    encoded.insert(encoded.end(), outputV.begin(), outputV.end());
    encoded.insert(encoded.end(), outputR.begin(), outputR.end());
    encoded.insert(encoded.end(), outputS.begin(), outputS.end());
    return encoded;
}



// WriteCallback used by sendData (Curl)
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

struct WriteThis
{
    const char *readptr;
    size_t sizeleft;
};


// read_callback used by sendData (Curl)
static size_t read_callback(void *dest, size_t size, size_t nmemb, void *userp)
{
    struct WriteThis *wt = (struct WriteThis *)userp;
    size_t buffer_size = size * nmemb;

    if (wt->sizeleft)
    {
        /* copy as much as possible from the source to the destination */
        size_t copy_this_much = wt->sizeleft;
        if (copy_this_much > buffer_size)
            copy_this_much = buffer_size;
        memcpy(dest, wt->readptr, copy_this_much);

        wt->readptr += copy_this_much;
        wt->sizeleft -= copy_this_much;
        return copy_this_much; /* we copied this many bytes */
    }

    return 0; /* no more data left to deliver */
}

// Send transaction by using Curl
int Transaction::sendData(std::string data, std::string api_endpoint)
{
    CURL *curl;
    CURLcode res;

    struct WriteThis wt;

    wt.readptr = data.c_str();
    wt.sizeleft = data.length();

    //std::cout << "Length data:" << (long)wt.sizeleft << std::endl;

    struct curl_slist *headers = NULL;
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, api_endpoint.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &wt);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (long)wt.sizeleft);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        if (VERBOSE)
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (VERBOSE)
            std::cout << "***Transaction sended***" << std::endl;
        if (VERBOSE)
            std::cout << "CURL STATUS CODE:" << res << std::endl;
        std::cout << "Response:" << std::endl;
        std::cout << readBuffer << std::endl;
    }
}