#include <fstream>
#include "longnum.h"


// приводит число LongNum к виду "0", а также избавляется от мусора в массивах
LongNum getLongNum() {
    LongNum num;
    num.sign = true;
    num.integerSize = 0;
    num.fractionSize = 0;
    num.firstFractionDigitLength = 0;
    for (int i = 0; i < DATA_SIZE; ++i) {
        num.integer[i] = 0;
        num.fraction[i] = 0;
    }
    return num;
}


bool isDigit(char a, bool zero = true) {
    if (zero) {
        return a >= '0' && a <= '7';
    } else {
        return a >= '1' && a <= '7';
    }
}


std::string reverse(std::string &line) {
    std::string reversed;
    for (int i = static_cast<int>(line.length()) - 1; i >= 0; --i) {
        reversed += line[i];
    }
    return reversed;
}


// проверяет, удовлетворяет ли строка синтаксису БНФ записи
bool isParsable(std::string &line) {
    unsigned int len = line.length();
    unsigned int i;
    bool isSign = true;
    bool isInteger = false;
    bool isFraction = false;

    for (i = 0; i < len; ++i) {
        if (isSign && (line[i] == '-' || isDigit(line[i]))) {
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


// возвращает целую часть числа, отбрасывая незначащие нули
std::string getInteger(std::string &line) {
    std::string integer;
    bool hasSignificantDigit = false;
    unsigned int len = line.length();
    int startPosition = (line[0] == '-') ? 1 : 0;
    int dotPosition = static_cast<int>(line.find('.'));
    int endPosition = (dotPosition > -1) ? dotPosition - 1: static_cast<int>(len) - 1;

    // сохраняет в integer только значащие цифры
    for (int i = startPosition; i <= endPosition; ++i) {
        if (!hasSignificantDigit && isDigit(line[i], false) ) {
            hasSignificantDigit = true;
        }
        if (hasSignificantDigit) {
            integer += line[i];
        }
    }

    return integer;
}


// возвращает дробную часть числа, отбрасывая незначащие нули
std::string getFraction(std::string &line) {
    std::string fraction;
    bool hasSignificantDigit = false;
    unsigned int len = line.length();
    int startPosition = static_cast<int>(len) - 1;
    int dotPosition = static_cast<int>(line.find('.'));
    int endPosition = (dotPosition > -1) ? dotPosition + 1: startPosition + 1;

    // сохраняет в fraction только значащие цифры
    for (int i = startPosition; i >= endPosition; --i) {
        if (!hasSignificantDigit && isDigit(line[i], false)) {
            hasSignificantDigit = true;
        }
        if (hasSignificantDigit) {
            fraction += line[i];
        }
    }

    return reverse(fraction);
}


bool readSign(std::ifstream &inFile, bool &sign) {
    std::string line;
    getline(inFile, line);
    if (line.length() == 2 && (line[0] == '+' || line[0] == '-')) {
        sign = line[0] == '+';
        return true;
    }
    return false;
}


// парсит строку, в которой должно находиться число LongNum, и при успехе преобразует строку в num
bool readLongNum(std::ifstream &inFile, LongNum &num) {
    std::string line;
    getline(inFile, line);
    if (static_cast<int>(line.find('\r')) > -1) {
        line.erase(line.length() - 1);
    }
    if (isParsable(line)) {
        std::string element;
        num = getLongNum();

        num.sign = line[0] != '-';
        std::string integer = getInteger(line);
        std::string fraction = getFraction(line);

        num.integerSize = 0;
        element = "";
        for (int i = static_cast<int>(integer.length() - 1); i >= 0; --i) {
            element += integer[i];
            if (element.length() == 2 || i == 0) {
                if (element == "00") {
                    num.integer[num.integerSize] = ZERO;
                } else {
                    element = reverse(element);
                    num.integer[num.integerSize] = std::stoi(element);
                }
                if (++num.integerSize == DATA_SIZE) {
                    break;
                }
                element = "";
            }
        }

        num.fractionSize = 0;
        element = "";
        for (int i = static_cast<int>(fraction.length() - 1); i >= 0; --i) {
            element += fraction[i];
            if (element.length() == 2 || i == 0) {
                if (element == "00") {
                    num.fraction[num.fractionSize] = ZERO;
                } else {
                    element = reverse(element);
                    num.fraction[num.fractionSize] = std::stoi(element);
                    num.firstFractionDigitLength = element.length();
                }
                if (++num.fractionSize == DATA_SIZE) {
                    break;
                }
                element = "";
            }
        }

        return true;
    }
    return false;
}


// выводит число LongNum в читаемом виде в конец файла outFile
void writeLongNum(std::ofstream &outFile, LongNum &num) {
    std::string strNum;
    if (!num.sign) {
        strNum += "-";
    }
    if (num.integerSize > 0) {
        for (int i = num.integerSize - 1; i >= 0; --i) {
            if (num.integer[i] != ZERO) {
                if (i < num.integerSize - 1 && num.integer[i] < 8) {
                    strNum += "0";
                }
                strNum += std::to_string(num.integer[i]);
            } else {
                strNum += "00";
            }
        }
    } else {
        strNum += "0";
    }
    if (num.fractionSize > 0) {
        strNum += ".";
        for (int i = num.fractionSize - 1; i >= 0; --i) {
            if (num.fraction[i] != ZERO) {
                if (num.firstFractionDigitLength == 2 && num.fraction[i] < 8) {
                    strNum += "0";
                }
                strNum += std::to_string(num.fraction[i]);
            } else {
                strNum += "00";
            }
        }
    }
    outFile << strNum;
}


LongNum sumLongNum(LongNum &a, LongNum &b) {
    return b;
}


LongNum subLongNum(LongNum &a, LongNum &b) {
    // написать
}