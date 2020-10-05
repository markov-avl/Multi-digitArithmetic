#include <iostream>
#include <fstream>
#include "constants.h"
#include "messages.h"
#include "longnum.h"


int main() {
    std::ifstream *inFile(IN_PATH);
    std::ofstream outFile(OUT_PATH);
    bool isFileCorrect = inFile.is_open();

    if (!isFileCorrect) {
        outFile << fileNotFound(IN_PATH);
    } else {
        unsigned int index = 0;
        bool sign = true;
        LongNum num;
        LongNum sum = getLongNum();

        while (!inFile.eof()) {
            if (index % 2 == 0) {
                if (!readLongNum(inFile, num)) {
                    outFile << invalidLongNum(index) << std::endl;
                    isFileCorrect = false;
                } else if (isFileCorrect) {
                    num.sign = !sign == !num.sign;
                    sum = sumLongNum(sum, num);
                }
            } else if (!readSign(inFile, sign)) {
                outFile << invalidSign(index) << std::endl;
                isFileCorrect = false;
            }
            index++;
        }

        if (index == 0) {
            outFile << fileIsEmpty(IN_PATH);
        } else if (index % 2 == 0 && isFileCorrect) {
            outFile << invalidEndOfFile(IN_PATH);
        } else {
            writeLongNum(outFile, sum);
        }
    }

    if (isFileCorrect) {
        std::cout << successfullyCompleted(IN_PATH, OUT_PATH);
    } else {
        std::cout << unsuccessfullyCompleted(IN_PATH, OUT_PATH);
    }

    inFile.close();
    outFile.close();

    return 0;
}