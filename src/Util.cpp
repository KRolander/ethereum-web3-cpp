//
//   Author: Roland Kromes, @ roland.kromes@univ-cotedazur.fr
//   Address: LEAT Laboratory, Université Côte d'Azur
//
//   The Util class contains tools for RLP encoding and hexa string representation encoding
//

#include "Util.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdint.h>

#include <string.h>
#include <cstring>

// returns output (header) length
uint32_t Util::RlpEncodeWholeHeader(uint8_t* header_output, uint32_t total_len) {
    if (total_len < 55) {
        header_output[0] = (uint8_t)0xc0 + (uint8_t)total_len;
        return 1;
    } else {
        uint8_t tmp_header[8];
        memset(tmp_header, 0, 8);
        uint32_t hexdigit = 1;
        uint32_t tmp = total_len;
        while ((uint32_t)(tmp / 256) > 0) {
            tmp_header[hexdigit] = (uint8_t)(tmp % 256);
            tmp = (uint32_t)(tmp / 256);
            hexdigit++;
        }
        tmp_header[hexdigit] = (uint8_t)(tmp);
        tmp_header[0] = (uint8_t)0xf7 + (uint8_t)hexdigit;

        // fix direction for header
        uint8_t header[8];
        memset(header, 0, 8);
        header[0] = tmp_header[0];
        for (int i=0; i<hexdigit; i++) {
            header[i+1] = tmp_header[hexdigit-i];
        }

        memcpy(header_output, header, (size_t)hexdigit+1);
        return hexdigit+1;
    }
}

std::vector<uint8_t> Util::RlpEncodeWholeHeaderWithVector(uint32_t total_len) {
    std::vector<uint8_t> header_output;
    if (total_len < 55) {
        header_output.push_back((uint8_t)0xc0 + (uint8_t)total_len);
    } else {
        std::vector<uint8_t> tmp_header;
        uint32_t hexdigit = 1;
        uint32_t tmp = total_len;
        while ((uint32_t)(tmp / 256) > 0) {
            tmp_header.push_back((uint8_t)(tmp % 256));
            tmp = (uint32_t)(tmp / 256);
            hexdigit++;
        }
        tmp_header.push_back((uint8_t)(tmp));
        tmp_header.insert(tmp_header.begin(), 0xf7 + (uint8_t)hexdigit);

        // fix direction for header
        std::vector<uint8_t> header;
        header.push_back(tmp_header[0]);
        for (int i=0; i<tmp_header.size()-1; i++) {
            header.push_back(tmp_header[tmp_header.size()-1-i]);
        }

        header_output.insert(header_output.end(), header.begin(), header.end());
    }
    return header_output;
}


// returns output length
uint32_t Util::RlpEncodeItem(uint8_t* output, const uint8_t* input, uint32_t input_len) {
    if (input_len==1 && input[0] == 0x00) {
        uint8_t c[1] = {0x80};
        memcpy(output, c, 1);
        return 1;
    } else if (input_len==1 && input[0] < 128) {
        memcpy(output, input, 1);
        return 1;
    } else if (input_len <= 55) {
        uint8_t _ = (uint8_t)0x80 + (uint8_t)input_len;
        uint8_t header[] = {_};
        memcpy(output, header, 1);
        memcpy(output+1, input, (size_t)input_len);
        return input_len+1;
    } else {
        uint8_t tmp_header[8];
        memset(tmp_header, 0, 8);
        uint32_t hexdigit = 1;
        uint32_t tmp = input_len;
        while ((uint32_t)(tmp / 256) > 0) {
            tmp_header[hexdigit] = (uint8_t)(tmp % 256);
            tmp = (uint32_t)(tmp / 256);
            hexdigit++;
        }
        tmp_header[hexdigit] = (uint8_t)(tmp);
        tmp_header[0] = (uint8_t)0xb7 + (uint8_t)hexdigit;

        // fix direction for header
        uint8_t header[8];
        memset(header, 0, 8);
        header[0] = tmp_header[0];
        for (int i=0; i<hexdigit; i++) {
            header[i+1] = tmp_header[hexdigit-i];
        }

        memcpy(output, header, hexdigit+1);
        memcpy(output+hexdigit+1, input, (size_t)input_len);
        return input_len+hexdigit+1;
    }
}

std::vector<uint8_t> Util::RlpEncodeItemWithVector(const std::vector<uint8_t> input) {
    std::vector<uint8_t> output;
    uint16_t input_len = input.size();

    if (input_len==1 && input[0] == 0x00) {
        output.push_back(0x80);
    } else if (input_len==1 && input[0] < 128) {
        output.insert(output.end(), input.begin(), input.end());
    } else if (input_len <= 55) {
        uint8_t _ = (uint8_t)0x80 + (uint8_t)input_len;
        output.push_back(_);
        output.insert(output.end(), input.begin(), input.end());
    } else {
       std::vector<uint8_t> tmp_header;
        uint32_t tmp = input_len;
        while ((uint32_t)(tmp / 256) > 0) {
            tmp_header.push_back((uint8_t)(tmp % 256));
            tmp = (uint32_t)(tmp / 256);
        }
        tmp_header.push_back((uint8_t)(tmp));
        uint8_t len = tmp_header.size();
        tmp_header.insert(tmp_header.begin(), 0xb7 + len);

        // fix direction for header
        std::vector<uint8_t> header;
        header.push_back(tmp_header[0]);
        uint8_t hexdigit = tmp_header.size() - 1;
        for (int i=0; i<hexdigit; i++) {
            header.push_back(tmp_header[hexdigit-i]);
        }

        output.insert(output.end(), header.begin(), header.end());
        output.insert(output.end(), input.begin(), input.end());
    }
    return output;
}

std::vector<uint8_t> Util::ConvertNumberToVector(uint32_t val) {
    std::vector<uint8_t> tmp;
    std::vector<uint8_t> ret;
    if ((uint32_t)(val / 256) >= 0) {
        while ((uint32_t)(val / 256) > 0) {
            tmp.push_back((uint8_t)(val % 256));
            val = (uint32_t)(val / 256);
        }
        tmp.push_back((uint8_t)(val % 256));
        uint8_t len = tmp.size();
        for (int i=0; i<len; i++) {
            ret.push_back(tmp[len-i-1]);
        }
    } else {
        ret.push_back((uint8_t)val);
    }
    return ret;
}



std::vector<uint8_t> Util::ConvertNumberToVector64(uint64_t val) {
    std::vector<uint8_t> tmp;
    std::vector<uint8_t> ret;
    if ((uint64_t)(val / 256) >= 0) {
        while ((uint64_t)(val / 256) > 0) {
            tmp.push_back((uint8_t)(val % 256));
            val = (uint64_t)(val / 256);
        }
        tmp.push_back((uint8_t)(val % 256));
        uint8_t len = tmp.size();
        for (int i=0; i<len; i++) {
            ret.push_back(tmp[len-i-1]);
        }
    } else {
        ret.push_back((uint8_t)val);
    }
    return ret;
}


uint32_t Util::ConvertNumberToUintArray(uint8_t *str, uint32_t val) {
    uint32_t ret = 0;
    uint8_t tmp[8];
    memset(tmp,0,8);
    if ((uint32_t)(val / 256) >= 0) {
        while ((uint32_t)(val / 256) > 0) {
            tmp[ret] = (uint8_t)(val % 256);
            val = (uint32_t)(val / 256);
            ret++;
        }
        tmp[ret] = (uint8_t)(val % 256);
        for (int i=0; i<ret+1; i++) {
            str[i] = tmp[ret-i];
        }
    } else {
        str[0] = (uint8_t)val;
    }

    return ret+1;
}

std::vector<uint8_t> Util::ConvertCharStrToVector(const uint8_t *in) {
    uint32_t ret = 0;
    uint8_t tmp[MAX_SIZE];
    strcpy((char *)tmp, (char *)in);
    std::vector<uint8_t> out;
    printf(" Here is tmp %s\n", tmp);

    // remove "0x"
    char * ptr = strtok((char*)tmp, "x");
    if (strlen(ptr)!=1) {
        ptr = (char *)tmp;
    } else {
        ptr = strtok(NULL, "x");
    }
    
    size_t lenstr = strlen(ptr);
    std::cout << "Lenght : " << lenstr << std::endl;

    for (int i=0; i<lenstr; i+=2) {
        char c[3];
        c[0] = *(ptr+i);
        c[1] = *(ptr+i+1);

        c[2] = 0x00;
        uint8_t val = strtol(c, nullptr, 16);
        out.push_back(val);
        std::cout << (uint32_t) out[ret] << ' ';

        ret++;
    }
    std::cout << std::endl;


    std::cout << "Lenght : " << lenstr << std::endl;


    return out;
}

std::vector<uint8_t> Util::ConvertStringToVector(const std::string* str) {
    std::cout << " ******** " << str->size() << std::endl;
    
    //  std::vector<uint8_t> test3(str->begin(), str->end());
    // uint8_t *test4 = test3.data();
    // std::vector<uint32_t> test = ConvertCharStrToVector(test4);
    // std::vector<uint8_t> test2;
    return ConvertCharStrToVector((uint8_t*)(str->c_str()));
}

uint32_t Util::ConvertCharStrToUintArray(uint8_t *out, const uint8_t *in) {
    uint32_t ret = 0;
    uint8_t tmp[256];
    strcpy((char *)tmp, (char *)in);

    // remove "0x"
    char * ptr = strtok((char*)tmp, "x");
    if (strlen(ptr)!=1) {
        ptr = (char *)tmp;
    } else {
        ptr = strtok(NULL, "x");
    }

    size_t lenstr = strlen(ptr);
    for (int i=0; i<lenstr; i+=2) {
        char c[3];
        c[0] = *(ptr+i);
        c[1] = *(ptr+i+1);
        c[2] = 0x00;
        uint8_t val = strtol(c, nullptr, 16);
        out[ret] = val;
        ret++;
    }
    return ret;
};

uint8_t Util::HexToInt(uint8_t s) {
    uint8_t ret = 0;
    if(s >= '0' && s <= '9'){
        ret = uint8_t(s - '0');
    } else if(s >= 'a' && s <= 'f'){
        ret = uint8_t(s - 'a' + 10);
    } else if(s >= 'A' && s <= 'F'){
        ret = uint8_t(s - 'A' + 10);
    }
    return ret;
}

void Util::BufToCharStr(char* str, const uint8_t* buf, uint32_t len) {
    sprintf(str, "0x");
    for (int i = 0; i < len; i++) {
        sprintf(str, "%s%02x", str, buf[i]);
    }
}

void Util::VectorToCharStr(char* str, const std::vector<uint8_t> buf) {
    sprintf(str, "0x");
    for (int i = 0; i < buf.size(); i++) {
        sprintf(str, "%s%02x", str, buf[i]);
    }
}

std::string Util::VectorToString(const std::vector<uint8_t> buf) {
    std::string ret = "0x";
    for (int i = 0; i < buf.size(); i++) {
        char v[3];
        memset(v, 0, sizeof(v));
        sprintf(v, "%02x", buf[i]);
        ret = ret + std::string(v);
    }
    return ret;
}

// Convert uint8_t to hexString

void Util::bytes2hex(unsigned char *src, char *out, int len)
{
    while (len--)
    {
        *out++ = HexLookUp[*src >> 4];
        *out++ = HexLookUp[*src & 0x0F];
        src++;
    }
    *out = 0;
}

// Convert uint16_t to hexString

void Util::doubleBytes2hex(uint16_t *src, char *out, int len)
{
    while (len--)
    {
        *out++ = HexLookUp[*src >> 12];
        *out++ = HexLookUp[(*src & 0x0F00) >> 8];   
        *out++ = HexLookUp[(*src & 0x00F0) >> 4];
        *out++ = HexLookUp[*src & 0x0F];
        src++;
    }
    *out = 0;
}

// Convert uint32_t to hexString


void Util::quarteBytes2hex(uint32_t *src, char *out, int len)
{
    while (len--)
    {
        *out++ = HexLookUp[(*src & 0xF0000000)>> 28];
        *out++ = HexLookUp[(*src & 0x0F000000) >> 24];   
        *out++ = HexLookUp[(*src & 0x00F00000) >> 20];
        *out++ = HexLookUp[(*src & 0x000F0000) >> 16];
        *out++ = HexLookUp[(*src & 0x0000F000) >> 12];
        *out++ = HexLookUp[(*src & 0x00000F00) >> 8];   
        *out++ = HexLookUp[(*src & 0x000000F0) >> 4];
        *out++ = HexLookUp[*src & 0x0F];
        src++;
    }
    *out = 0;
}