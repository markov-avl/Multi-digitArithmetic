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

bool readLongNum(std::ifstream &inFile, LongNum &num);
void writeLongNum(std::ofstream &outFile, LongNum &num);
bool isEqual(LongNum &a, LongNum &b);
bool isLess(LongNum &a, LongNum &b);
LongNum sumLongNum(LongNum &a, LongNum &b);
LongNum subLongNum(LongNum &a, LongNum &b);


#endif