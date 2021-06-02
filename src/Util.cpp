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
#include <iterator>

// returns output (header) length
uint32_t Util::RlpEncodeWholeHeader(uint8_t *header_output, uint32_t total_len)
{
    if (total_len < 55)
    {
        header_output[0] = (uint8_t)0xc0 + (uint8_t)total_len;
        return 1;
    }
    else
    {
        uint8_t tmp_header[8];
        memset(tmp_header, 0, 8);
        uint32_t hexdigit = 1;
        uint32_t tmp = total_len;
        while ((uint32_t)(tmp / 256) > 0)
        {
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
        for (int i = 0; i < hexdigit; i++)
        {
            header[i + 1] = tmp_header[hexdigit - i];
        }

        memcpy(header_output, header, (size_t)hexdigit + 1);
        return hexdigit + 1;
    }
}

std::vector<uint8_t> Util::RlpEncodeWholeHeaderWithVector(uint32_t total_len)
{
    std::vector<uint8_t> header_output;
    if (total_len < 55)
    {
        header_output.push_back((uint8_t)0xc0 + (uint8_t)total_len);
    }
    else
    {
        std::vector<uint8_t> tmp_header;
        uint32_t hexdigit = 1;
        uint32_t tmp = total_len;
        while ((uint32_t)(tmp / 256) > 0)
        {
            tmp_header.push_back((uint8_t)(tmp % 256));
            tmp = (uint32_t)(tmp / 256);
            hexdigit++;
        }
        tmp_header.push_back((uint8_t)(tmp));
        tmp_header.insert(tmp_header.begin(), 0xf7 + (uint8_t)hexdigit);

        // fix direction for header
        std::vector<uint8_t> header;
        header.push_back(tmp_header[0]);
        for (int i = 0; i < tmp_header.size() - 1; i++)
        {
            header.push_back(tmp_header[tmp_header.size() - 1 - i]);
        }

        header_output.insert(header_output.end(), header.begin(), header.end());
    }
    return header_output;
}

// returns output length
uint32_t Util::RlpEncodeItem(uint8_t *output, const uint8_t *input, uint32_t input_len)
{
    if (input_len == 1 && input[0] == 0x00)
    {
        uint8_t c[1] = {0x80};
        memcpy(output, c, 1);
        return 1;
    }
    else if (input_len == 1 && input[0] < 128)
    {
        memcpy(output, input, 1);
        return 1;
    }
    else if (input_len <= 55)
    {
        uint8_t _ = (uint8_t)0x80 + (uint8_t)input_len;
        uint8_t header[] = {_};
        memcpy(output, header, 1);
        memcpy(output + 1, input, (size_t)input_len);
        return input_len + 1;
    }
    else
    {
        uint8_t tmp_header[8];
        memset(tmp_header, 0, 8);
        uint32_t hexdigit = 1;
        uint32_t tmp = input_len;
        while ((uint32_t)(tmp / 256) > 0)
        {
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
        for (int i = 0; i < hexdigit; i++)
        {
            header[i + 1] = tmp_header[hexdigit - i];
        }

        memcpy(output, header, hexdigit + 1);
        memcpy(output + hexdigit + 1, input, (size_t)input_len);
        return input_len + hexdigit + 1;
    }
}


int Util::RlpEncodeItemWithArray(uint8_t *inputUint, uint32_t input_len)
{
    if (input_len == 1 && inputUint[0] == 0x00)
    {
        uint8_t output[1];
        output[0] = 0x80;
        
        return input_len + 1;
    }
    else if (input_len == 1 && inputUint[0] < 128)
    {
        uint8_t output[1];
        output[0] = inputUint[0];
        return input_len + 1;
    }
    else if (input_len <= 55)
    {
        uint8_t output[55];
        uint8_t _ = (uint8_t)0x80 + (uint8_t)input_len;
        output[0] = _;
        int t;
        int h = 1;
        for (t = 0; t < input_len; t++)
        {
            output[h] = inputUint[t];
            h++;
        }
        std::cout << "_________Hulk__________" << std::endl;
        int p;
        for (p = 0; p < h; p++)
        {
            std::cout << (uint32_t)output[p] << " ";
            // inputUint[p] = input[p];
        }
       
        return input_len + h;
    }
    else
    {
        uint8_t tmp_header[MAX_SIZE];
        uint32_t tmp = input_len;
        std::cout << "Size : " << tmp << std::endl;

        int k = 0;
        while ((uint32_t)(tmp / 256) > 0)
        {
            tmp_header[k] = (uint8_t)(tmp % 256);
            k++;
            tmp = (uint32_t)(tmp / 256);
            std::cout << " k = " << k << std::endl;
        }
        tmp_header[k] = tmp;
        uint8_t len = k + 1;
        int p;
        for (p = 0; p < len; p++)
        {
            std::cout << (uint32_t)tmp_header[p] << " ";
            // inputUint[p] = input[p];
        }
        //tmp_header[k] = 0xb7 + len;
        uint8_t header[MAX_SIZE];

        header[0] = 0xb7 + len;
        uint8_t hexdigit = len - 1;
        int l = 1;

        for (int i = len - 1; i >= 0; i--)
        {
            header[l] = tmp_header[i];
            l++;
        }
        std::cout << "_________Hulk__________" << std::endl;

        uint8_t output[len + k + input_len];
        for (p = 0; p < len + k; p++)
        {
            std::cout << (uint32_t)header[p] << " ";
            output[p] = header[p];
            // inputUint[p] = input[p];
        }
        int b = len + k;
        for (p = 0; p < input_len; p++)
        {
            std::cout << (uint32_t)inputUint[p] << " ";
            output[b] = inputUint[p];
            b++;
            // inputUint[p] = input[p];
        }
        std::cout << std::endl;

        return len + k + input_len;
    }

}

std::vector<uint8_t> RlpEncodeItemWithVector_v3(uint8_t *inputUint, uint32_t input_len)
{

    if (input_len == 1 && inputUint[0] == 0x00)
    {
        uint8_t output[1];
        output[0] = 0x80;
        std::vector<uint8_t> outputT(output, output + 1);
        return outputT;
    }
    else if (input_len == 1 && inputUint[0] < 128)
    {
        uint8_t output[1];
        output[0] = inputUint[0];
        std::vector<uint8_t> outputT(output, output + 1);
        return outputT;
    }
    else if (input_len <= 55)
    {
        uint8_t output[55];
        uint8_t _ = (uint8_t)0x80 + (uint8_t)input_len;
        output[0] = _;
        int t;
        int h = 1;
        for (t = 0; t < input_len; t++)
        {
            output[h] = inputUint[t];
            h++;
        }
        std::cout << "_________Hulk__________" << std::endl;
        int p;
        for (p = 0; p < h; p++)
        {
            std::cout << (uint32_t)output[p] << " ";
            // inputUint[p] = input[p];
        }
        std::vector<uint8_t> outputT(output, output + h);
        return outputT;
    }
    else
    {
        uint8_t tmp_header[MAX_SIZE];
        uint32_t tmp = input_len;
        std::cout << "Size : " << tmp << std::endl;

        int k = 0;
        while ((uint32_t)(tmp / 256) > 0)
        {
            tmp_header[k] = (uint8_t)(tmp % 256);
            k++;
            tmp = (uint32_t)(tmp / 256);
            std::cout << " k = " << k << std::endl;
        }
        tmp_header[k] = tmp;
        uint8_t len = k + 1;
        int p;
        for (p = 0; p < len; p++)
        {
            std::cout << (uint32_t)tmp_header[p] << " ";
            // inputUint[p] = input[p];
        }
        //tmp_header[k] = 0xb7 + len;
        uint8_t header[MAX_SIZE];

        header[0] = 0xb7 + len;
        uint8_t hexdigit = len - 1;
        int l = 1;

        for (int i = len - 1; i >= 0; i--)
        {
            header[l] = tmp_header[i];
            l++;
        }
        std::cout << "_________Hulk__________" << std::endl;

        uint8_t output[len + k + input_len];
        for (p = 0; p < len + k; p++)
        {
            std::cout << (uint32_t)header[p] << " ";
            output[p] = header[p];
            // inputUint[p] = input[p];
        }
        int b = len + k;
        for (p = 0; p < input_len; p++)
        {
            std::cout << (uint32_t)inputUint[p] << " ";
            output[b] = inputUint[p];
            b++;
            // inputUint[p] = input[p];
        }
        std::cout << std::endl;

        std::vector<uint8_t> outputT(output, output + len + k + input_len);
        return outputT;
    }
}

std::vector<uint8_t> Util::RlpEncodeItemWithVector(const std::vector<uint8_t> input)
{

    // std::cout << "_________Iron Man__________" << std::endl;
    // uint32_t input_len_v = input.size();

    // uint8_t inputUint[input_len_v];
    // int p;
    // for(p=0; p<input_len_v; p++)
    // {
    //     // std::cout << (uint32_t) input[p] <<" ";
    //     inputUint[p] = input[p];
    // }
    // std::cout << std::endl;
    // uint8_t outputTMP[MAX_SIZE];

    // std::vector<uint8_t> output = RlpEncodeItemWithVector_v3(inputUint, input_len_v);
    // std::cout << "_____ RlpEncodeItemWithVector_View again ___ View again " << std::endl;

    // std::vector<uint8_t> outputT(outputTMP, outputTMP + input_len_v + 1);

    // for(p=0; p<v.size(); p++)
    // {
    //     std::cout << (uint32_t) v[p] << ' ';
    // }
    // std::cout << std::endl;

    // std::vector<uint8_t> output(outputTMP, outputTMP + input_len_v);

    std::vector<uint8_t> output;
    uint16_t input_len = input.size();

    if (input_len == 1 && input[0] == 0x00)
    {
        output.push_back(0x80);
    }
    else if (input_len == 1 && input[0] < 128)
    {
        output.insert(output.end(), input.begin(), input.end());
    }
    else if (input_len <= 55)
    {
        uint8_t _ = (uint8_t)0x80 + (uint8_t)input_len;
        output.push_back(_);
        output.insert(output.end(), input.begin(), input.end());
    }
    else
    {
        std::vector<uint8_t> tmp_header;
        uint32_t tmp = input_len;
        while ((uint32_t)(tmp / 256) > 0)
        {
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
        for (int i = 0; i < hexdigit; i++)
        {
            header.push_back(tmp_header[hexdigit - i]);
        }

        output.insert(output.end(), header.begin(), header.end());
        output.insert(output.end(), input.begin(), input.end());
    }

    // std::cout << "_____ RlpEncodeItemWithVector ____ Vecotors" << std::endl;
    // int k;
    // for (k = 0; k < output.size(); k++)
    //     std::cout << (uint32_t)output[k] << ' ';
    // std::cout << std::endl;

    return output;
}

int Util::convertUint32ToUint8Array(uint32_t in_number, uint8_t *out_number)
{   
    int arraySize;
    int k=0;
    uint8_t tmpArray[4];
    if((in_number >> 8) >= 0)
    {
        while ((uint32_t)(in_number >> 8) > 0)
        {   
            // std::cout << "Here" << std::endl;
            tmpArray[k] = uint8_t (in_number & 0xff);
            // std::cout << "   tmpArray[k] : " << (uint32_t) tmpArray[k] << std::endl;

            k++;
            in_number = in_number >> 8;
        }
        tmpArray[k] = uint8_t (in_number & 0xff);
        //  std::cout << "   tmpArray[k] : " << (uint32_t) tmpArray[k] << std::endl;
        arraySize = k+1;
        int i;
        int lastElement = arraySize - 1;
        for(i=0; i<arraySize; i++)
        {
            out_number[i] =tmpArray[lastElement - i];
        }
    }
    else
    {
        out_number[0] = uint8_t (in_number & 0xff);
        arraySize = 1;
    }
    return arraySize;

}


int Util::convertUint64ToUint8Array(uint64_t in_number, uint8_t *out_number)
{   
    int arraySize;
    int k=0;
    uint8_t tmpArray[8];
    if((in_number >> 8) >= 0)
    {
        while ((uint64_t)(in_number >> 8) > 0)
        {   
            // std::cout << "Here" << std::endl;
            tmpArray[k] = uint8_t (in_number & 0xff);
            // std::cout << "   tmpArray[k] : " << (uint32_t) tmpArray[k] << std::endl;

            k++;
            in_number = in_number >> 8;
        }
        tmpArray[k] = uint8_t (in_number & 0xff);
        //  std::cout << "   tmpArray[k] : " << (uint32_t) tmpArray[k] << std::endl;
        arraySize = k+1;
        int i;
        int lastElement = arraySize - 1;
        for(i=0; i<arraySize; i++)
        {
            out_number[i] =tmpArray[lastElement - i];
        }
    }
    else
    {
        out_number[0] = uint8_t (in_number & 0xff);
        arraySize = 1;
    }
    return arraySize;

}


std::vector<uint8_t> Util::ConvertNumberToVector(uint32_t val)
{
    std::vector<uint8_t> tmp;
    std::vector<uint8_t> ret;
    if ((uint32_t)(val / 256) >= 0)
    {
        while ((uint32_t)(val / 256) > 0)
        {
            tmp.push_back((uint8_t)(val % 256));
            val = (uint32_t)(val / 256);
        }
        tmp.push_back((uint8_t)(val % 256));
        uint8_t len = tmp.size();
        for (int i = 0; i < len; i++)
        {
            ret.push_back(tmp[len - i - 1]);
        }
    }
    else
    {
        ret.push_back((uint8_t)val);
    }
    return ret;
}

std::vector<uint8_t> Util::ConvertNumberToVector64(uint64_t val)
{
    std::vector<uint8_t> tmp;
    std::vector<uint8_t> ret;
    if ((uint64_t)(val / 256) >= 0)
    {
        while ((uint64_t)(val / 256) > 0)
        {
            tmp.push_back((uint8_t)(val % 256));
            val = (uint64_t)(val / 256);
        }
        tmp.push_back((uint8_t)(val % 256));
        uint8_t len = tmp.size();
        for (int i = 0; i < len; i++)
        {
            ret.push_back(tmp[len - i - 1]);
        }
    }
    else
    {
        ret.push_back((uint8_t)val);
    }
    return ret;
}

uint32_t Util::ConvertNumberToUintArray(uint8_t *str, uint32_t val)
{
    uint32_t ret = 0;
    uint8_t tmp[8];
    memset(tmp, 0, 8);
    if ((uint32_t)(val / 256) >= 0)
    {
        while ((uint32_t)(val / 256) > 0)
        {
            tmp[ret] = (uint8_t)(val % 256);
            val = (uint32_t)(val / 256);
            ret++;
        }
        tmp[ret] = (uint8_t)(val % 256);
        for (int i = 0; i < ret + 1; i++)
        {
            str[i] = tmp[ret - i];
        }
    }
    else
    {
        str[0] = (uint8_t)val;
    }

    return ret + 1;
}

std::vector<uint8_t> Util::ConvertCharStrToVector(const uint8_t *in)
{
    uint32_t ret = 0;
    uint8_t tmp[MAX_SIZE];
    strcpy((char *)tmp, (char *)in);
    std::vector<uint8_t> out;

    printf(" Here is tmp %s\n", tmp);

    // remove "0x"
    char *ptr = strtok((char *)tmp, "x");
    if (strlen(ptr) != 1)
    {
        ptr = (char *)tmp;
    }
    else
    {
        ptr = strtok(NULL, "x");
    }

    size_t lenstr = strlen(ptr);
    // uint8_t out[lenstr];
    std::cout << "Lenght : " << lenstr << std::endl;
    // int j=0;
    for (int i = 0; i < lenstr; i += 2)
    {
        char c[3];
        c[0] = *(ptr + i);
        c[1] = *(ptr + i + 1);

        c[2] = 0x00;
        uint8_t val = strtol(c, nullptr, 16);
        out.push_back(val);

        std::cout << (uint32_t)out[ret] << ' ';

        ret++;
    }
    std::cout << std::endl;

    std::cout << "Lenght : " << lenstr << std::endl;
    // x, x + sizeof x / sizeof x[0]
    // std::vector<uint8_t> outter(out, out + sizeof out / sizeof out[0]);
    return out;
}

std::vector<uint8_t> Util::ConvertStringToVector(const std::string *str)
{
    // std::cout << " ******** " << str->size() << std::endl;

    // return out;

    std::string strTmp(str[0]);

    strTmp.erase(0, 2);

    int bytes_n = strTmp.size() / 2;

    //std::vector<uint8_t> dest(bytes_n);
    uint8_t dest[bytes_n];

    // std::cout << " ______ Iron Man _______" << std::endl;

    char *source = (char *)strTmp.c_str();

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

    // int i;
    // for(i=0; i<hexStrSize; i++)
    //     std::cout << (uint32_t) convertToUint8[i];
    // std::cout << std::endl;

    // std::vector<uint8_t> outter(convertToUint8, convertToUint8 + sizeof convertToUint8 / sizeof convertToUint8[0]);
    std::vector<uint8_t> v(dest, dest + bytes_n);
    // v.assign(dest, dest + bytes_n);
    return v;

    // return ConvertCharStrToVector((uint8_t*)(str->c_str()));
}

void Util::ConvertStringToArray(std::string *str, uint8_t *dest, int sizeOfArray)
{
    // std::cout << " ******** " << str->size() << std::endl;

    // return out;

    // std::string strTmp(str[0]);

    str->erase(0, 2);

    // int bytes_n = strTmp.size() / 2;

    // //std::vector<uint8_t> dest(bytes_n);
    // uint8_t dest[bytes_n];

    // std::cout << " ______ Iron Man _______" << std::endl;

    char *source = (char *)str->c_str();

    int i;
    int j = 0;
    for (i = 0; i < sizeOfArray; i++)
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

uint32_t Util::ConvertCharStrToUintArray(uint8_t *out, const uint8_t *in)
{
    uint32_t ret = 0;
    uint8_t tmp[256];
    strcpy((char *)tmp, (char *)in);

    // remove "0x"
    char *ptr = strtok((char *)tmp, "x");
    if (strlen(ptr) != 1)
    {
        ptr = (char *)tmp;
    }
    else
    {
        ptr = strtok(NULL, "x");
    }

    size_t lenstr = strlen(ptr);
    for (int i = 0; i < lenstr; i += 2)
    {
        char c[3];
        c[0] = *(ptr + i);
        c[1] = *(ptr + i + 1);
        c[2] = 0x00;
        uint8_t val = strtol(c, nullptr, 16);
        out[ret] = val;
        ret++;
    }
    return ret;
};

uint8_t Util::HexToInt(uint8_t s)
{
    uint8_t ret = 0;
    if (s >= '0' && s <= '9')
    {
        ret = uint8_t(s - '0');
    }
    else if (s >= 'a' && s <= 'f')
    {
        ret = uint8_t(s - 'a' + 10);
    }
    else if (s >= 'A' && s <= 'F')
    {
        ret = uint8_t(s - 'A' + 10);
    }
    return ret;
}

void Util::BufToCharStr(char *str, const uint8_t *buf, uint32_t len)
{
    sprintf(str, "0x");
    for (int i = 0; i < len; i++)
    {
        sprintf(str, "%s%02x", str, buf[i]);
    }
}

void Util::VectorToCharStr(char *str, const std::vector<uint8_t> buf)
{
    sprintf(str, "0x");
    for (int i = 0; i < buf.size(); i++)
    {
        sprintf(str, "%s%02x", str, buf[i]);
    }
}

std::string Util::VectorToString(const std::vector<uint8_t> buf)
{
    std::string ret = "0x";
    for (int i = 0; i < buf.size(); i++)
    {
        char v[3];
        memset(v, 0, sizeof(v));
        sprintf(v, "%02x", buf[i]);
        ret = ret + std::string(v);
    }
    return ret;
}

void Util::singleBytes2hex(unsigned char *src, char *out)
{

    *out++ = HexLookUp[*src >> 4];
    *out++ = HexLookUp[*src & 0x0F];
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
        *out++ = HexLookUp[(*src & 0xF0000000) >> 28];
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