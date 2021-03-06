//
//   Author: Roland Kromes, @ roland.kromes@univ-cotedazur.fr
//   Address: LEAT Laboratory, Université Côte d'Azur
//
//   The Util class contains tools for RLP encoding and hexa string representation encoding
//

#ifndef WEB3_UTIL_H
#define WEB3_UTIL_H

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string.h>
#include <cstring>

#include <sstream>
#include <iomanip>
#include <boost/algorithm/hex.hpp>

#define MAX_SIZE 16384

static char HexLookUp[] = "0123456789abcdef";

class Util {
public:
    // RLP implementation
    // reference:
    //     https://github.com/ethereum/wiki/wiki/%5BEnglish%5D-RLP
    static uint32_t        RlpEncodeWholeHeader(uint8_t *header_output, uint32_t total_len);
    static std::vector<uint8_t> RlpEncodeWholeHeaderWithVector(uint32_t total_len);
    static uint32_t        RlpEncodeItem(uint8_t* output, const uint8_t* input, uint32_t input_len);
    static std::vector<uint8_t> RlpEncodeItemWithVector(const std::vector<uint8_t> input);


    static int RlpEncodeItemWithArray(uint8_t *inputUint, uint32_t input_len);

    static uint32_t        ConvertNumberToUintArray(uint8_t *str, uint32_t val);
    static std::vector<uint8_t> ConvertNumberToVector(uint32_t val);
        static std::vector<uint8_t> ConvertNumberToVector64(uint64_t val);

    static uint32_t        ConvertCharStrToUintArray(uint8_t *out, const uint8_t *in);
    static std::vector<uint8_t> ConvertCharStrToVector(const uint8_t *in);
    static std::vector<uint8_t> ConvertStringToVector(const std::string* str);

    static void ConvertStringToArray(std::string* str, uint8_t *dest, int sizeOfArray);
    static int convertUint32ToUint8Array(uint32_t in_number, uint8_t *out_number);
    static int convertUint64ToUint8Array(uint64_t in_number, uint8_t *out_number);


    static uint8_t HexToInt(uint8_t s);
    static void    BufToCharStr(char* str, const uint8_t* buf, uint32_t len);
    static void    VectorToCharStr(char* str, const std::vector<uint8_t> buf);
    static std::string  VectorToString(const std::vector<uint8_t> buf);

    static void bytes2hex(unsigned char *src, char *out, int len);
    static void singleBytes2hex(unsigned char *src, char *out);

    static void doubleBytes2hex(uint16_t *src, char *out, int len);
    static void quarteBytes2hex(uint32_t *src, char *out, int len);



};

#endif //WEB3_UTIL_H