//
//   Author: Roland Kromes, @ roland.kromes@univ-cotedazur.fr
//   Address: LEAT Laboratory, Université Côte d'Azur
//
//   The Util class contains tools for RLP encoding and hexa string representation encoding
//

#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string.h>
#include <cstring>
#include "Transaction.h"

#include "Util.h"
#include "crypto/sha3.h"

#define MAX_SIZE 4096

class Payload {
public:
  
  std::string r;
  std::string s;

  //  sign(Hash(data || ID)) with privateKey, return signature{s, r}
  void signedPayload(std::string data, uint8_t ID, std::string privKey);
  void Test();

  


int chhex(char ch);

void HexStrToUchar(unsigned char *dest, const char *source, int bytes_n);



};

#endif //PAYLOAD_H
