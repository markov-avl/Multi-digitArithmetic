#include <iostream>
#include "constants.h"

#ifndef LAB1_LONGNUM_H
#define LAB1_LONGNUM_H


typedef struct {
    bool sign;
    unsigned char integer[DATA_SIZE];
    unsigned char fraction[DATA_SIZE];
    unsigned char integerSize;
    unsigned char fractionSize;
} LongNum;

LongNum getLongNum();
bool readSign(std::ifstream *inFile, bool &sign);
bool readLongNum(std::ifstream *inFile, LongNum &num);
void writeLongNum(std::ofstream *inFile, LongNum &sum);
LongNum sumLongNum(LongNum &a, LongNum &b);
LongNum subLongNum(LongNum &a, LongNum &b);


#endif