#include <iostream>
#include <fstream>
#include "constants.h"
#include "messages.h"
#include "longnum.h"


int main() {
    std::ifstream inFile(IN_PATH);
    std::ofstream outFile(OUT_PATH);
    bool isFileCorrect = inFile.is_open();
    LongNum sum = getLongNum();
    LongNum num = getLongNum();

    readLongNum(inFile, sum);
    readLongNum(inFile, num);
    std::cout << isLess(sum, num) << std::endl;

    if (!isFileCorrect) {
        outFile << fileNotFound(IN_PATH);
    } else {
        unsigned int index = 1;
        bool sign = true;
        LongNum num;

        while (!inFile.eof()) {
            if (index % 2 == 1) {
                if (!readLongNum(inFile, num)) {
                    outFile << invalidLongNum(index) << std::endl;
                    isFileCorrect = false;
                } else if (isFileCorrect) {
                    sum = (sign) ? sumLongNum(sum, num) : subLongNum(sum, num);
                }
            } else if (!readSign(inFile, sign)) {
                outFile << invalidSign(index) << std::endl;
                isFileCorrect = false;
            }
            ++index;
        }

        if (index == 1) {
            outFile << fileIsEmpty(IN_PATH);
            isFileCorrect = false;
        } else if (index % 2 == 1 && isFileCorrect) {
            outFile << invalidEndOfFile(IN_PATH);
            isFileCorrect = false;
        }
    }

    if (isFileCorrect) {
        writeLongNum(outFile, sum);
        std::cout << successfullyCompleted(IN_PATH, OUT_PATH);
    } else {
        std::cout << unsuccessfullyCompleted(IN_PATH, OUT_PATH);
    }

    inFile.close();
    outFile.close();

    return 0;
}