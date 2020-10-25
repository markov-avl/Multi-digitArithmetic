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
    unsigned char data[DATA_SIZE]{};

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

    // возвращает экспоненту числа, представленного в виде строки (строка должна быть без незначащих нулей)
    static int getExponent(std::string &line) {
        int exponent;
        int length = static_cast<int>( line.length() );
        int dotPos = static_cast<int>( line.find('.') );
        int start = line[0] == '-' ? 1 : 0;

        if (dotPos > -1) {
            if (line.find("0.") == start) {
                exponent = -1;
                for (int i = 2 + start; i < length && line[i] == '0'; ++i) {
                    --exponent;
                }
            } else {
                exponent = dotPos - start - 1;
            }
        } else {
            exponent = length - start - 1;
        }

        return exponent;
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
    static void removeInsignificantZeroes(std::string &line) {
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

    // возращает число в виде строки, будто домноженного до целого (если оно дробное)
    static void turnToInteger(std::string &line) {
        int dotPos = static_cast<int>( line.find('.') );
        if (dotPos > -1) {
            int start = line[0] == '-' ? 1 : 0;
            if (line.find("0.") == start) {
                int amount = 2;
                int length = static_cast<int>( line.length() );
                for (int i = 2 + start; i < length && line[i] == '0'; ++i) {
                    ++amount;
                }
                line.erase(start, amount);
            } else {
                line.erase(dotPos, 1);
            }
        }
    }

    static void pushData(unsigned char data[], int &size, int n) {
        int shift = n / 2;
        int index = size + shift - 1;

        size += shift + ( (n % 2 == 1 && data[size - 1] / 10 > 0) ? 1 : 0 );

        if (index >= DATA_SIZE) {
            index = DATA_SIZE - 1;
        }
        if (shift > 0) {
            while (index - shift >= 0) {
                data[index] = data[index - shift];
                --index;
            }
            for (int i = 0; i < shift; ++i) {
                data[index] = 0;
            }
        }
        if (n % 2 == 1) {
            unsigned char dataCopy[DATA_SIZE];
            for (int i = 0; i < DATA_SIZE; ++i) {
                dataCopy[i] = data[i];
            }
            data[0] = dataCopy[0] % 10 * 10;
            for (int i = 1; i < DATA_SIZE; ++i) {
                data[i] = dataCopy[i] % 10 * 10 + dataCopy[i - 1] / 10;
            }
        }
    }

    public:

    LongNum() {
        sign = true;
        exponent = 1;
        size = 1;
    }

    // парсит строку, в которой должно находиться число LongNum, и при успехе преобразует строку в num
    bool readLongNum(std::ifstream &inFile) {
        std::string line;
        getline(inFile, line);
        if ( static_cast<int>(line.find('\r') ) > -1) {
            line.erase(line.length() - 1);
        }
        if ( isParsable(line) ) {
            removeInsignificantZeroes(line);

            int start = line[0] == '-' ? 1 : 0;

            sign = start == 0;
            exponent = getExponent(line);
            size = 0;

            turnToInteger(line);

            std::string number;
            int index = static_cast<int>(line.length() - 1);

            while (index >= start && size < DATA_SIZE) {
                number += line[index];
                if (number.length() == 2 || index == start) {
                    data[size++] = static_cast<unsigned char>( std::stoi( getReversedString(number) ) );
                    number.erase(0, 2);
                }
                --index;
            }

            return true;
        }
        return false;
    }

    void writeLongNum(std::ofstream &outFile) {
        int i;
        int index = size - 1;

        if (!sign) {
            outFile << '-';
        }
        if (exponent < 0) {
            outFile << "0.";
            i = -1;
            while (i > exponent) {
                outFile << '0';
                --i;
            }
        } else {
            i = exponent;
            while (i >= 0 && exponent > 0) {
                if (i == 0) {
                    outFile << static_cast<short>(data[index] / 10) << '.' << static_cast<short>(data[index] % 10);
                } else {
                    outFile << static_cast<short>(data[index]);
                }
                i -= 2;
                --index;
            }
            if (index >= 0 && size > 1) {
                outFile << '.';
            }
        }

        for (; index >= 0; --index) {
            if (data[index] < 10 && index != size - 1) {
                outFile << '0';
            }
            outFile << static_cast<short>(data[index]);
        }

        outFile << std::endl;
    }

    bool isEqual(LongNum &a) {
        if (sign == a.sign && size == a.size && exponent == a.exponent) {
            for (int i = 0; i < DATA_SIZE; ++i) {
                if (data[i] != a.data[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    bool isLess(LongNum &a) {
        if (sign > a.sign || exponent > a.exponent) {
            return false;
        } else if (sign == a.sign && exponent == a.exponent) {
            int index = (size > a.size) ? size : a.size;
            for (int i = index - 1; i >= 0; --i) {
                if (data[i] > a.data[i]) {
                    return false;
                } else if (data[i] < a.data[i]) {
                    return true;
                }
            }
            return false;
        }
        return true;
    }

    LongNum sumLongNum(LongNum &a) {
        if (sign == a.sign) {
            int exp1 = exponent;
            int exp2 = a.exponent;
            int exp = exp1 > exp2 ? exp1 : exp2;

            unsigned char data1[DATA_SIZE];
            unsigned char data2[DATA_SIZE];
            for (int i = 0; i < DATA_SIZE; ++i) {
                data1[i] = data[i];
                data2[i] = a.data[i];
            }

            if (exp1 != exp) {
                pushData(data1, size, exp - exp1);
                exp1 = exp;
            }
            if (exp2 != exp) {
                pushData(data2, a.size, exp - exp2);
                exp2 = exp;
            }

            size_t size = max(d1.size(), d2.size()); // определяем максимальный размер векторов

            // выравниваем размеры векторов, добавляя нули в конец каждого из них
            while (d1.size() != size)
                d1.push_back(0);

            while (d2.size() != size)
                d2.push_back(0);

            size_t len = 1 + size;

            LongDouble res; // создаём новое число

            res.sign = sign; // знак результата совпадает со знаком чисел
            res.digits = vector<int>(len, 0); // создаём вектор из len нулей

            // заполняем каждую ячейку вектора суммой двух соответствующих цифр
            for (size_t i = 0; i < size; i++)
                res.digits[i + 1] = d1[i] + d2[i];

            // проверяем переполнения
            for (size_t i = len - 1; i > 0; i--) {
                res.digits[i - 1] += res.digits[i] / 10; // прибавляем к более старшему разряду десятки текущего
                res.digits[i] %= 10; // оставляем у текущего разряда только единицы
            }

            res.exponent = exp + 1; // восстанавливаем экспоненту
            res.removeZeroes(); // убираем нули

            return res; // возвращаем число
        }

        if (sign == -1)
            return x - (-(*this)); // если первое число отрицательное, то из второго вычитаем первое с обратным знаком

        return *this - (-x);









//        if (sign == a.sign) {
//            if (exponent != a.exponent) {
//                if (exponent > a.exponent) {
//                    unsigned char dataCopy[DATA_SIZE] = {};
//                    for (int i = 0; i < a.size; ++i) {
//                        dataCopy[i] = a.data[i];
//                    }
//                    pushData(dataCopy, a.size, exponent - a.exponent);
//                    for (int i = 0; i < DATA_SIZE; ++i) {
//                        data[i] = dataCopy[i] + data[i];
//                    }
//                } else {
//                    unsigned char dataCopy[DATA_SIZE] = {};
//                    for (int i = 0; i < size; ++i) {
//                        dataCopy[i] = data[i];
//                    }
//                    pushData(dataCopy, size, a.exponent - exponent);
//                    for (int i = 0; i < DATA_SIZE; ++i) {
//                        data[i] = dataCopy[i] + a.data[i];
//                    }
//                    exponent = a.exponent;
//                }
//            } else {
//
//            }
//
//
//            int exp = (exponent > a.exponent) ? exponent : a.exponent;
//
//        }
    }

} LongNum;

#endif