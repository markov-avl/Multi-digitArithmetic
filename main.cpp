#include <iostream>
#include "src/constants.h"
#include "src/messages.h"
#include "src/longnum.h"

bool readSign(std::ifstream& inFile, bool& sign) {
    std::string line;
    getline(inFile, line);
    if (static_cast<int>( line.find('\r') ) > -1) {
        line.erase(line.length() - 1);
    }
    if (line.length() == 1 && (line[0] == '+' || line[0] == '-') ) {
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
        int responseCode;
        bool sign = true;
        unsigned int index = 1;

        while ( !inFile.eof() ) {
            if (index % 2 == 1) {
                LongNum num;
                responseCode = readLongNum(inFile, num);
                if (responseCode == SUCCESS && isFileCorrect) {
                    if (index == 1) {
                        sum = num;
                    } else {
                        if (sign) {
                            sum = sumLongNum(sum, num, responseCode);
                        } else {
                            sum = subLongNum(sum, num, responseCode);
                        }
                        if (responseCode != SUCCESS) {
                            isFileCorrect = false;
                            if (responseCode == INTEGER_OVERFLOWED) {
                                outFile << operationIntegerOverflowed(index - 1) << std::endl;
                            } else  {
                                outFile << operationFractionOverflowed(index - 1) << std::endl;
                            }
                        }
                    }
                } else {
                    isFileCorrect = false;
                    if (responseCode == UNPARSABLE) {
                        outFile << unparsable(index) << std::endl;
                    } else if (responseCode == MINUS_ZERO) {
                        outFile << minusZero(index) << std::endl;
                    } else if (responseCode == INTEGER_OVERFLOWED) {
                        outFile << integerOverflowed(index) << std::endl;
                    } else if (responseCode == FRACTION_OVERFLOWED) {
                        outFile << fractionOverflowed(index) << std::endl;
                    } else if (responseCode == INTEGER_AND_FRACTION_OVERFLOWED)  {
                        outFile << integerAndFractionOverflowed(index) << std::endl;
                    }
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