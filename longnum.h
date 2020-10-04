#include <iostream>
#include "constants.h"

#ifndef LAB1_LONGNUM_H
#define LAB1_LONGNUM_H


typedef struct {
    bool sign;
    unsigned char integer[DATA_SIZE];
    unsigned char fraction[DATA_SIZE];
} LongNum;

LongNum getLongNum();
bool readLongNum(std::string &line, LongNum &num);
void writeLongNum(std::ofstream &outFile, LongNum &sum);
LongNum sumLongNum(LongNum &a, LongNum &b);
LongNum subLongNum(LongNum &a, LongNum &b);


#endif