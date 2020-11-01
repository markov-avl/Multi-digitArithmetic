#include <iostream>
#include "constants.h"
#include "messages.h"
#include "longnum.h"


int main() {
    std::ifstream inFile(IN_PATH);
    std::ofstream outFile(OUT_PATH);
    bool isFileCorrect = inFile.is_open();

    if ( !inFile.is_open() ) {
        outFile << fileNotFound(IN_PATH);
        isFileCorrect = false;
    } else {
        unsigned int index = 1;
        bool sign = true;
        LongNum num1, num2, sum;

        readLongNum(inFile, num1);
        readLongNum(inFile, num2);
        sum = absoluteSum(num1, num2);
        writeLongNum(outFile, sum);


        while ( !inFile.eof() ) {
            if (index % 2 == 1) {
                if ( !readLongNum(inFile, num1) ) {
                    outFile << invalidLongNum(index) << std::endl;
                    isFileCorrect = false;
                } else if (isFileCorrect) {
                    writeLongNum(outFile, num1);
                    sum = num1.sign ? sumLongNum(sum, num1) : subLongNum(sum, num1);
                }
            } else if ( !readSign(inFile, sign) ) {
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
        } else {
            writeLongNum(outFile, sum);
        }
    }

    inFile.close();
    outFile.close();

    if (isFileCorrect) {
        std::cout << successfullyCompleted(IN_PATH, OUT_PATH);
    } else {
        std::cout << unsuccessfullyCompleted(IN_PATH, OUT_PATH);
    }

    return 0;
}