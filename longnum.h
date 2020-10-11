#include <iostream>
#include "constants.h"

#ifndef LAB1_LONGNUM_H
#define LAB1_LONGNUM_H


typedef struct {
    bool sign;
    unsigned char data[DATA_SIZE];
    unsigned char size;
    unsigned char lastIntUnit;
    unsigned char lastFractUnit;
} LongNum;

LongNum getLongNum();
bool readSign(std::ifstream &inFile, bool &sign);

bool readLongNum(std::ifstream &inFile, LongNum &num);
void writeLongNum(std::ofstream &outFile, LongNum &num);
LongNum sumLongNum(LongNum &a, LongNum &b);
LongNum subLongNum(LongNum &a, LongNum &b);
bool isEqual(LongNum &a, LongNum &b);
bool isLess(LongNum &a, LongNum &b);

#endif