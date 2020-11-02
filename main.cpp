#include <iostream>
#include "constants.h"
#include "messages.h"
#include "longnum.h"


bool readSign(std::ifstream& inFile, bool& sign) {
    std::string line;
    getline(inFile, line);
    if (line.length() == 2 && (line[0] == '+' || line[0] == '-') ) {
        sign = line[0] == '+';
        return true;
    }
    return false;
}


bool process(std::ifstream& inFile, std::ofstream& outFile) {
    bool isFileCorrect = inFile.is_open();

    if (!isFileCorrect) {
        outFile << fileNotFound(IN_PATH);
    } else {
        LongNum sum;
        bool sign = true;
        unsigned int index = 1;

        while ( !inFile.eof() ) {
            if (index % 2 == 1) {
                LongNum num;
                if ( !readLongNum(inFile, num) ) {
                    outFile << invalidLongNum(index) << std::endl;
                    isFileCorrect = false;
                }
                if (isFileCorrect) {
                    sum = sign ? sumLongNum(sum, num) : subLongNum(sum, num);
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
        } else if (isFileCorrect) {
            writeLongNum(outFile, sum);
        }
    }

    return isFileCorrect;
}


int main() {
    std::ifstream inFile(IN_PATH);
    std::ofstream outFile(OUT_PATH);
    bool isSuccessfullyCompleted = process(inFile, outFile);

    inFile.close();
    outFile.close();

    if (isSuccessfullyCompleted) {
        std::cout << successfullyCompleted(IN_PATH, OUT_PATH) << std::endl;
    } else {
        std::cout << unsuccessfullyCompleted(IN_PATH, OUT_PATH) << std::endl;
    }

    return 0;
}