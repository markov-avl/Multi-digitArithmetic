#include <iostream>
#include <fstream>
#include "messages.h"
#include "longnum.h"


LongNum getSum(bool isFileCorrect, bool sign, LongNum &sum, LongNum &num) {
    /*
     *  мы производим сумму, если файл корректен
     *  если производим, то:
     *      знак num нужно изменить (или оставить как есть) в соответствие с sign;
     *      отправить sum и num в функцию суммирования;
     *      вернуть результат суммирования
     */
}

// bux fix

bool readSign(std::string &line, bool &sign) {
    if (line.length() == 2 && (line[0] == '+' || line[0] == '-')) {
        sign = line[0] == '+';
        return true;
    }
    return false;
}


bool process(std::ifstream &inFile, std::ofstream &outFile, const char* &inPath) {
    bool isFileCorrect = true;
    if (!inFile.is_open()) {
        outFile << fileNotFound(inPath);
        isFileCorrect = false;
    } else {
        unsigned int index = 0;
        bool sign = true;
        std::string line;
        LongNum num;
        LongNum sum = getLongNum();

        while (!inFile.eof())
        {
            getline(inFile, line);
            if (index % 2 == 0) {
                if (readLongNum(line, num)) {
                    sum = getSum(isFileCorrect, sign, sum, num);
                } else {
                    outFile << invalidLongNum(index) << std::endl;
                    isFileCorrect = false;
                }
            } else if (!readSign(line, sign)) {
                outFile << invalidSign(index) << std::endl;
                isFileCorrect = false;
            }
            index++;
        }

        if (index == 0) {
            outFile << fileIsEmpty(inPath);
        } else if (index % 2 == 0 && isFileCorrect) {
            outFile << invalidEndOfFile(inPath);
        } else {
            writeLongNum(outFile, sum);
        }
    }
    return isFileCorrect;
}