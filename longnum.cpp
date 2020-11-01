#include <fstream>
#include "longnum.h"

bool readSign(std::ifstream &inFile, bool &sign) {
    std::string line;
    getline(inFile, line);
    if (line.length() == 2 && (line[0] == '+' || line[0] == '-')) {
        sign = line[0] == '+';
        return true;
    }
    return false;
}


// проверка, является ли символ числом восьмеричной СС
bool isDigit(char a, bool withZero = true) {
    if (withZero) {
        return a >= '0' && a <= '7';
    } else {
        return a >= '1' && a <= '7';
    }
}

// проверка, удовлетворяет ли строка синтаксису БНФ записи
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

// разворачивает строку ("4321" -> "1234")
std::string reverse(std::string &line) {
    std::string reversed;
    for (int i = static_cast<int>(line.length()) - 1; i >= 0; --i) {
        reversed += line[i];
    }
    return reversed;
}

// возвращает целую часть числа, отбрасывая незначащие нули
std::string getInteger(std::string &line) {
    std::string integer;
    bool hasSignificantDigit = false;
    unsigned int len = line.length();
    int startPosition = (line[0] == '-') ? 1 : 0;
    int dotPosition = static_cast<int>( line.find('.') );
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


// парсит строку, в которой должно находиться число LongNum, и при успехе преобразует строку в num
bool readLongNum(std::ifstream &inFile, LongNum &num) {
    std::string line;
    getline(inFile, line);
    if (static_cast<int>(line.find('\r')) > -1) {
        line.erase(line.length() - 1);
    }
    if ( isParsable(line) ) {
        std::string element;

        num.sign = line[0] != '-';
        std::string integerString = getInteger(line);
        std::string fractionString = getFraction(line);
        int integerLength = static_cast<int>( integerString.length() );
        int fractionLength = static_cast<int>( fractionString.length() );

        num.integerSize = 0;
        element = "";
        for (int i = integerLength - 1; i >= 0; --i) {
            element += integerString[i];
            if (element.length() == 2 || i == 0) {
                element = reverse(element);
                num.integer[num.integerSize] = std::stoi(element);
                if (++num.integerSize == DATA_SIZE) {
                    break;
                }
                element.clear();
            }
        }

        num.fractionSize = 0;
        element = "";
        for (int i = 0; i < fractionLength; ++i) {
            element += fractionString[i];
            if (element.length() == 2 || i == 0) {
                element = reverse(element);
                num.fraction[num.fractionSize] = std::stoi(element);
                if (++num.fractionSize == DATA_SIZE) {
                    break;
                }
                element.clear();
            }
        }

        return true;
    }
    return false;
}

// выводит число LongNum в читаемом виде в конец файла outFile
void writeLongNum(std::ofstream &outFile, LongNum &num) {
    if (!num.sign) {
        outFile << '-';
    }

    if (num.integerSize > 0) {
        for (int i = num.integerSize - 1; i >= 0; --i) {
            if (num.integer[i] < 10 && i != num.integerSize - 1) {
                outFile << '0';
            }
            outFile << num.integer[i];
        }
    } else {
        outFile << '0';
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
    // написать
}


LongNum subLongNum(LongNum &a, LongNum &b) {
    // написать
}