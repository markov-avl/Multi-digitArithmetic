#include <fstream>
#include "longnum.h"


// приводит число LongNum к виду "0", а также избавляется от мусора в массивах
LongNum getLongNum() {
    LongNum num;
    num.sign = true;
    num.size = 0;
    num.lastIntUnit = 0;
    num.lastFractUnit = 0;
    for (unsigned char & i : num.data) {
        i = 0;
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
        int endPos;
        num = getLongNum();

        num.sign = line[0] != '-';
        std::string integer = getInteger(line);
        std::string fraction = getFraction(line);

        if (integer.length() > 0) {
            endPos = static_cast<int>(integer.length()) - 1;
            for (int i = 0; i <= endPos; ++i) {
                element += integer[i];
                if (element.length() == 2 || i == endPos) {
                    num.data[num.size] = std::stoi(element);
                    num.lastIntUnit = element.length();
                    if (++num.size == DATA_SIZE) {
                        break;
                    }
                    element = "";
                }
            }
        } else {
            ++num.size;
            num.lastIntUnit = 1;
        }

        if (num.size < DATA_SIZE - 1 && fraction.length() > 0) {
            num.data[num.size++] = DOT;
            endPos = static_cast<int>(fraction.length()) - 1;
            for (int i = 0; i <= endPos; ++i) {
                element += fraction[i];
                if (element.length() == 2 || i == endPos) {
                    num.data[num.size] = std::stoi(element);
                    num.lastFractUnit = element.length();
                    if (++num.size == DATA_SIZE) {
                        break;
                    }
                    element = "";
                }
            }
        }

        return true;
    }
    return false;
}


// выводит число LongNum в читаемом виде в конец файла outFile
void writeLongNum(std::ofstream &outFile, LongNum &num) {
    std::string strNum;
    bool hasFract = false;
    int unit;

    if (!num.sign) {
        strNum += "-";
    }
    if (num.size > 1) {
        for (int i = 0; i < num.size; ++i) {
            if (num.data[i] != DOT) {
                if (hasFract) {
                    unit = (i + 1 == num.size) ? num.lastFractUnit : 2;
                } else {
                    unit = (i + 1 == num.size || i + 1 < DATA_SIZE && num.data[i + 1] == DOT) ? num.lastIntUnit : 2;
                }
                if (unit == 2 && num.data[i] < 8) {
                    strNum += "0";
                }
                strNum += std::to_string(num.data[i]);
            } else {
                hasFract = true;
                strNum += ".";
            }
        }
    } else {
        strNum += std::to_string(num.data[0]);
    }
    outFile << strNum;
}


LongNum sumLongNum(LongNum &a, LongNum &b) {
    return b;
}


LongNum subLongNum(LongNum &a, LongNum &b) {
    // написать
}

bool isEqual(LongNum &a, LongNum &b) {
    int i = -1;
    if (a.sign == b.sign && a.size == b.size && a.lastIntUnit == b.lastIntUnit && a.lastFractUnit == b.lastFractUnit) {
        for (i = 0; i < a.size && a.data[i] == b.data[i]; ++i);
    }
    return i == a.size;
}

bool isLess(LongNum &a, LongNum &b) {
    if (a.sign < b.sign) {
        return true;
    } else {
        int aSize, bSize;
        for (aSize = 0; aSize < a.size && a.data[aSize] != DOT; ++aSize);
        for (bSize = 0; bSize < b.size && b.data[bSize] != DOT; ++bSize);
        if (a.sign && aSize < bSize || !a.sign && aSize > bSize) {
            return true;
        } else {
            int aUnit, bUnit;
            for (int i = 0; i < aSize; ++i) {
                aUnit = (i + 1 == aSize || i + 1 < DATA_SIZE && a.data[i + 1] == DOT) ? a.lastIntUnit : 2;
                bUnit = (i + 1 == bSize || i + 1 < DATA_SIZE && b.data[i + 1] == DOT) ? b.lastIntUnit : 2;
                if (a.sign && (a.data[i] == b.data[i] && aUnit < bUnit || a.data[i] < b.data[i] && aUnit <= bUnit) ||
                        !a.sign && (a.data[i] == b.data[i] && aUnit > bUnit || a.data[i] > b.data[i] && aUnit >= bUnit)) {
                    return true;
                } else if (a.sign && a.data[i] > b.data[i] && aUnit >= bUnit ||
                        !a.sign && a.data[i] < b.data[i] && aUnit <= bUnit) {
                    return false;
                }
            }
            if (aSize < a.size && bSize < b.size) {
                if (a.sign && a.size - aSize < b.size - bSize || !a.sign && a.size - aSize > b.size - bSize) {
                    return true;
                } else {
                    for (int i = aSize + 1; i < a.size; ++i) {
                        aUnit = (i + 1 == a.size) ? a.lastFractUnit : 2;
                        bUnit = (i + 1 == b.size) ? b.lastFractUnit : 2;
                        if (a.sign && (a.data[i] == b.data[i] && aUnit > bUnit || a.data[i] < b.data[i] && aUnit <= bUnit) ||
                                !a.sign && (a.data[i] == b.data[i] && aUnit < bUnit || a.data[i] > b.data[i] && aUnit >= bUnit)) {
                            return true;
                        } else if (a.sign && a.data[i] < b.data[i] && aUnit >= bUnit ||
                                !a.sign && a.data[i] > b.data[i] && aUnit <= bUnit) {
                            return false;
                        }
                    }
                    return false;
                }
            } else {
                return a.size < b.size;
            }
        }
    }
}