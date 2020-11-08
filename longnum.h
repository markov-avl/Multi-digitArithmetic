#include <iostream>
#include <fstream>
#include "constants.h"

typedef struct LongNum {
    bool sign;
    unsigned char integer[DATA_SIZE]{};
    unsigned char fraction[DATA_SIZE]{};
    unsigned char integerSize{};
    unsigned char fractionSize{};

    LongNum() {
        sign = true;
        integerSize = 1;
    }
} LongNum;

bool readLongNum(std::ifstream& inFile, LongNum& num);
void writeLongNum(std::ofstream& outFile, LongNum& num);
bool isEqual(LongNum& a, LongNum& b, bool abs = false);
bool isLess(LongNum& a, LongNum& b, bool abs = false);
LongNum sumLongNum(LongNum& a, LongNum& b);
LongNum subLongNum(LongNum& a, LongNum& b);