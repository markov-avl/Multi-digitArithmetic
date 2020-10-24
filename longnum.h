#include <iostream>
#include <fstream>
#include "constants.h"

#ifndef LAB1_LONGNUM_H
#define LAB1_LONGNUM_H


typedef struct LongNum {

    static constexpr unsigned char DATA_SIZE = 150;

    private:

    bool sign;
    int exponent;
    int size;
    unsigned char data[DATA_SIZE] {};

    // проверка, является ли символ цифрой восьмеричной СС
    static bool isDigit(char a, bool withZero = true) {
        if (withZero) {
            return a >= '0' && a <= '7';
        } else {
            return a >= '1' && a <= '7';
        }
    }

    // проверка, удовлетворяет ли строка синтаксису БНФ записи
    static bool isParsable(std::string &line) {
        unsigned int len = line.length();
        unsigned int i;
        bool isSign = true;
        bool isInteger = false;
        bool isFraction = false;

        for (i = 0; i < len; ++i) {
            if ( isSign && ( line[i] == '-' || isDigit(line[i]) ) ) {
                isSign = false;
                isInteger = true;
            } else if (isInteger && line[i - 1] != '-' && line[i] == '.') {
                isInteger = false;
                isFraction = true;
            } else if ( (isInteger || isFraction) && isDigit(line[i]) ) {
                continue;
            } else {
                break;
            }
        }

        return i == len && line[i - 1] != '-' && line[i - 1] != '.' && isDigit(line[i - 1]);
    }

    // возвращает инвентированную строку ("1234" -> "4321")
    static std::string getReversedString(std::string &line) {
        std::string reversed;
        for (int i = static_cast<int>(line.length()) - 1; i >= 0; --i) {
            reversed += line[i];
        }
        return reversed;
    }

    // удаляет незначащие нули в числе
    static void removeInsignificantZeros(std::string &line) {
        int amount, start, index;
        int length = line.length();

        // удаляет незначащие нули в целой части числа
        amount = 0;
        start = line[0] == '-' ? 1 : 0;
        index = start;
        while (index + 1 < length && line[index] == '0' && line[index + 1] != '.') {
            ++amount;
            ++index;
        }
        if (amount > 0) {
            line.erase(start, amount);
            length -= amount;
        }

        // удаляет незначащие нули в дробной части числа
        start = static_cast<int>( line.find('.') );
        if (index > -1) {
            amount = 0;
            index = length - 1;
            while (line[index] == '0') {
                ++amount;
                --index;
            }
            line[index] == '.' ? line.erase(start, length - 1) : line.erase(index + 1, length - 1);
        }
    }

    public:

    LongNum() {
        sign = true;
        exponent = 1;
        size = 1;
        for (unsigned char & i : data) {
            i = 0;
        }
    }

    // парсит строку, в которой должно находиться число LongNum, и при успехе преобразует строку в num
    bool readLongNum(std::ifstream &inFile) {
        std::string line;
        getline(inFile, line);
        if ( static_cast<int>(line.find('\r') ) > -1) {
            line.erase(line.length() - 1);
        }
        if ( isParsable(line) ) {
            removeInsignificantZeros(line);

            std::string number;
            int index = static_cast<int>(line.length() - 1);
            int start = line[0] == '-' ? 1 : 0;

            sign = start == 0;
            exponent = static_cast<int>( line.find('.') );
            size = 0;

            while (index >= start && size < DATA_SIZE) {
                if (line[index] != '.') {
                    number += line[index];
                    if (number.length() == 2 || index == start) {
                        data[size++] = static_cast<unsigned char>( std::stoi( getReversedString(number) ) );
                        number = "";
                    }
                }
                --index;
            }
            return true;
        }
        return false;
    }

} LongNum;

#endif