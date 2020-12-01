#include <fstream>
#include "longnum.h"
#include "messages.h"


// проверка, является ли символ числом восьмеричной СС
bool isDigit(char a, bool withZero = true) {
    return (withZero ? a >= '0' && a <= '7' : a >= '1' && a <= '7');
}


// проверка, удовлетворяет ли строка синтаксису БНФ записи
bool isParsable(std::string& line) {
    unsigned int len = line.length();
    unsigned int i;
    bool isSign = true;
    bool isInteger = false;
    bool isFraction = false;

    for (i = 0; i < len; ++i) {
        if (isSign && ( line[i] == '-' || isDigit( line[i]) ) ) {
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
std::string reverse(std::string& line) {
    std::string reversed;
    for (int i = static_cast<int>( line.length() ) - 1; i >= 0; --i) {
        reversed += line[i];
    }
    return reversed;
}


// возвращает целую часть числа, отбрасывая незначащие нули
std::string getInteger(std::string& line) {
    std::string integer;
    bool hasSignificantDigit = false;
    unsigned int len = line.length();
    int startPosition = line[0] == '-' ? 1 : 0;
    int dotPosition = static_cast<int>( line.find('.') );
    int endPosition = dotPosition > -1 ? dotPosition - 1 : static_cast<int>(len) - 1;

    // сохраняет в integer только значащие цифры
    for (int i = startPosition; i <= endPosition; ++i) {
        if (!hasSignificantDigit && isDigit(line[i], false) || i == endPosition) {
            hasSignificantDigit = true;
        }
        if (hasSignificantDigit) {
            integer += line[i];
        }
    }

    return integer;
}


// возвращает дробную часть числа, отбрасывая незначащие нули
std::string getFraction(std::string& line) {
    std::string fraction;
    bool hasSignificantDigit = false;
    unsigned int len = line.length();
    int startPosition = static_cast<int>(len) - 1;
    int dotPosition = static_cast<int>( line.find('.') );
    int endPosition = dotPosition > -1 ? dotPosition + 1 : startPosition + 1;

    // сохраняет в fraction только значащие цифры
    for (int i = startPosition; i >= endPosition; --i) {
        if (!hasSignificantDigit && isDigit(line[i], false) ) {
            hasSignificantDigit = true;
        }
        if (hasSignificantDigit) {
            fraction += line[i];
        }
    }

    return reverse(fraction);
}


// если remainder получился 1, тогда произошло переполнение десятков и единиц в сумме
void sumNumbers(unsigned char& sum, int& remainder, int a, int b) {
    int units, tens, q1, q2;
    units = a % 10 + b % 10 + remainder;
    tens = a / 10 + b / 10 + (units >= 8 ? 1 : 0);
    q2 = units % 8;
    q1 = tens % 8;
    remainder = tens >= 8 ? 1 : 0;
    sum = q1 * 10 + q2;
}


// если remainder получился -1, тогда произошла нехватка десятков и единиц в разности
void subNumbers(unsigned char& sub, int& remainder, int a, int b) {
    int units, tens, q1, q2;
    units = a % 10 - b % 10 + remainder;
    tens = a / 10 - b / 10 - (units >= 0 ? 0 : 1);
    q2 = units >= 0 ? units : 8 + units;
    q1 = tens >= 0 ? tens : 8 + tens;
    remainder = tens >= 0 ? 0 : -1;
    sub = q1 * 10 + q2;
}


// складывает целую и дробную части, игнорируя знак (то есть вычитает по модулю)
LongNum absSum(LongNum& a, LongNum& b, int& responseCode) {
    LongNum sum;
    int remainder = 0;

    sum.integerSize = a.integerSize > b.integerSize ? a.integerSize : b.integerSize;
    sum.fractionSize = a.fractionSize > b.fractionSize ? a.fractionSize : b.fractionSize;

    for (int i = sum.fractionSize - 1; i >= 0; --i) {
        sumNumbers(sum.fraction[i], remainder, a.fraction[i], b.fraction[i]);
    }
    for (int i = 0; i < sum.integerSize; ++i) {
        sumNumbers(sum.integer[i], remainder, a.integer[i], b.integer[i]);
    }

    if (remainder > 0) {
        if (sum.integerSize < DATA_SIZE) {
            responseCode = SUCCESS;
            sum.integer[sum.integerSize++] = remainder;
        } else {
            responseCode = INTEGER_OVERFLOWED;
        }
    } else {
        responseCode = SUCCESS;
    }

    // удаление незначащих нулей, которые могут образоваться
    while (sum.integerSize > 1 && sum.integer[sum.integerSize - 1] == 0) {
        --sum.integerSize;
    }
    while (sum.fractionSize > 0 && sum.fraction[sum.fractionSize - 1] == 0) {
        --sum.fractionSize;
    }

    return sum;
}


// вычитает целую и дробную части, игнорируя знак (то есть складывает по модулю)
LongNum absSub(LongNum& a, LongNum& b, int& responseCode) {
    LongNum sub;
    int remainder = 0;

    sub.integerSize = a.integerSize > b.integerSize ? a.integerSize : b.integerSize;
    sub.fractionSize = a.fractionSize > b.fractionSize ? a.fractionSize : b.fractionSize;

    bool less = isLess(a, b, true);
    LongNum& lower = less ? a : b;
    LongNum& higher = less ? b : a;

    while (sub.fractionSize > 0 && higher.fraction[sub.fractionSize - 1] == lower.fraction[sub.fractionSize - 1]) {
        sub.fraction[--sub.fractionSize] = 0;
    }
    for (int i = sub.fractionSize - 1; i >= 0; --i) {
        subNumbers(sub.fraction[i], remainder, higher.fraction[i], lower.fraction[i]);
    }
    for (int i = 0; i < sub.integerSize; ++i) {
        subNumbers(sub.integer[i], remainder, higher.integer[i], lower.integer[i]);
    }

    responseCode = SUCCESS;

    // удаление незначащих нулей, которые могут образоваться
    while (sub.integerSize > 1 && sub.integer[sub.integerSize - 1] == 0) {
        --sub.integerSize;
    }
    while (sub.fractionSize > 0 && sub.fraction[sub.fractionSize - 1] == 0) {
        --sub.fractionSize;
    }

    return sub;
}


// парсит строку, в которой должно находиться число LongNum, и при успехе преобразует строку в num
int readLongNum(std::ifstream& inFile, LongNum& num) {
    std::string line;
    int code;
    getline(inFile, line);
    if (static_cast<int>( line.find('\r') ) > -1) {
        line.erase(line.length() - 1);
    }
    if ( isParsable(line) ) {
        std::string element;

        num.sign = line[0] != '-';
        std::string integerString = getInteger(line);
        std::string fractionString = getFraction(line);
        int integerLength = static_cast<int>( integerString.length() );
        int fractionLength = static_cast<int>( fractionString.length() );
        bool isIntegerOverflowed = integerLength > DATA_SIZE * 2;
        bool isFractionOverflowed = fractionLength > DATA_SIZE * 2;

        if (!isIntegerOverflowed && !isFractionOverflowed) {
            num.integerSize = 0;
            element = "";
            for (int i = integerLength - 1; i >= 0; --i) {
                element += integerString[i];
                if (element.length() == 2 || i == 0) {
                    element = reverse(element);
                    num.integer[num.integerSize++] = std::stoi(element);
                    element.clear();
                }
            }

            num.fractionSize = 0;
            element = "";
            for (int i = 0; i < fractionLength; ++i) {
                element += fractionString[i];
                if (element.length() == 2 || i == fractionLength - 1) {
                    num.fraction[num.fractionSize] = std::stoi(element);
                    if (i == fractionLength - 1 && element.length() == 1) {
                        num.fraction[num.fractionSize] *= 10;
                    }
                    ++num.fractionSize;
                    element.clear();
                }
            }

            if (!num.sign && num.integerSize == 1 && num.integer[0] == 0 && num.fractionSize == 0) {
                return MINUS_ZERO;
            } else {
                return SUCCESS;
            }
        } else if (isIntegerOverflowed && !isFractionOverflowed) {
            return INTEGER_OVERFLOWED;
        } else if (!isIntegerOverflowed && isFractionOverflowed) {
            return FRACTION_OVERFLOWED;
        } else {
            return INTEGER_AND_FRACTION_OVERFLOWED;
        }
    }
    return UNPARSABLE;
}


// выводит число LongNum в читаемом виде в конец файла outFile
void writeLongNum(std::ofstream& outFile, LongNum& num) {
    if (!num.sign) {
        outFile << '-';
    }
    for (int i = num.integerSize - 1; i >= 0; --i) {
        if (num.integer[i] < 10 && i < num.integerSize - 1) {
            outFile << '0';
        }
        outFile << static_cast<short>(num.integer[i]);
    }
    if (num.fractionSize > 0) {
        outFile << '.';
        for (int i = 0; i < num.fractionSize; ++i) {
            if (i < num.fractionSize - 1 || i == num.fractionSize - 1 && num.fraction[i] % 10 > 0) {
                if (num.fraction[i] < 10) {
                    outFile << '0';
                }
                outFile << static_cast<short>(num.fraction[i]);
            } else {
                outFile << static_cast<short>(num.fraction[i] / 10);
            }
        }
    }
    outFile << std::endl;
}


bool isEqual(LongNum& a, LongNum& b, bool abs) {
    if ( (a.sign == b.sign || abs) && a.integerSize == b.integerSize && a.fractionSize == b.fractionSize) {
        for (int i = a.integerSize - 1; i >= 0; --i) {
            if (a.integer[i] != b.integer[i]) {
                return false;
            }
        }
        for (int i = 0; i < a.fractionSize; ++i) {
            if (a.fraction[i] != b.fraction[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}


bool isLess(LongNum& a, LongNum& b, bool abs) {
    if (a.sign == b.sign || abs) {
        LongNum& lower = a.sign || abs ? a : b;
        LongNum& higher = a.sign || abs ? b : a;
        if (lower.integerSize == higher.integerSize) {
            for (int i = lower.integerSize - 1; i >= 0; --i) {
                if (lower.integer[i] != higher.integer[i]) {
                    return lower.integer[i] < higher.integer[i];
                }
            }
            int maxFractionSize = a.fractionSize > b.fractionSize ? a.fractionSize : b.fractionSize;
            for (int i = 0; i < maxFractionSize; ++i) {
                if (lower.fraction[i] != higher.fraction[i]) {
                    return lower.fraction[i] < higher.fraction[i];
                }
            }
            return false;
        } else {
            return lower.integerSize < higher.integerSize;
        }
    }
    return !a.sign && b.sign;
}


LongNum sumLongNum(LongNum& a, LongNum& b, int& responseCode) {
    LongNum sum;
    if (a.sign == b.sign) {
        sum = absSum(a, b, responseCode);
        sum.sign = a.sign;
    } else {
        if ( !isEqual(a, b, true) ) {
            bool less = isLess(a, b, true);
            sum = less ? absSub(b, a, responseCode) : absSub(a, b, responseCode);
            sum.sign = less ? b.sign : a.sign;
        }
    }
    return sum;
}


LongNum subLongNum(LongNum& a, LongNum& b, int& responseCode) {
    LongNum sub;
    if (a.sign == b.sign) {
        if ( !isEqual(a, b, true) ) {
            bool less = isLess(a, b, true);
            sub = less ? absSub(b, a, responseCode) : absSub(a, b, responseCode);
            sub.sign = less ? !b.sign : a.sign;
        }
    } else {
        sub = absSum(a, b, responseCode);
        sub.sign = a.sign;
    }
    return sub;
}