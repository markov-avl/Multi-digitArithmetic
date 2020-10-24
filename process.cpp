#include "longnum.h"
#include "messages.h"

bool readSign(std::ifstream &inFile, bool &sign) {
    std::string line;
    getline(inFile, line);
    if ( static_cast<int>(line.find('\r') ) > -1) {
        line.erase(line.length() - 1);
    }
    if (line.length() == 1 && (line[0] == '+' || line[0] == '-') ) {
        sign = line[0] == '+';
        return true;
    }
    return false;
}

bool processFile(std::ifstream &inFile, std::ofstream &outFile) {
    bool isFileCorrect = true;
    bool sign = true;
    unsigned int index = 1;
    LongNum sum, num;

    while ( !inFile.eof() ) {
        if (index % 2 == 1) {
            if (!num.readLongNum(inFile)) {
                outFile << invalidLongNum(index) << std::endl;
                isFileCorrect = false;
            } else if (isFileCorrect) {
                //sum = (sign) ? sumLongNum(sum, num) : subLongNum(sum, num);
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

    return isFileCorrect;
}