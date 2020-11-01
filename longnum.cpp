#include <fstream>
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


// проверка, является ли символ числом восьмеричной СС
bool isDigit(char a, bool withZero = true) {
    if (withZero) {
        return a >= '0' && a <= '7';
    } else {
        return a >= '1' && a <= '7';
    }
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


void sumNumbers(unsigned char& sum, int& remainder, int a, int b) {
    int mod, div, q1, q2;
    mod = a % 10 + b % 10;
    div = a / 10 + b / 10;
    q2 = (mod + remainder) % 8;
    q1 = (div + mod / 8) % 8;
    remainder = (div + mod / 8) / 8;
    sum = q1 * 10 + q2;
}


void subNumbers(unsigned char& sub, int& remainder, int a, int b) {
    int mod, div, q1, q2;
    mod = a % 10 - b % 10;
    div = a / 10 - b / 10;
    int k = mod % 8;
    int n = mod / 8;
    q2 = mod % 8 + (a > b ? 0 : 8);
    q1 = (div + mod / 8 + (a > b ? 0 : 8 + div) ) % 8;
    remainder = a > b ? 0 : -1;
    sub = abs(q1 * 10 + q2);

}


// складывает целую и дробную части, игнорируя знак
LongNum absoluteSum(LongNum& a, LongNum& b) {
    LongNum sum;
    int remainder = 0;

    sum.integerSize = a.integerSize > b.integerSize ? a.integerSize : b.integerSize;
    sum.fractionSize = a.fractionSize > b.fractionSize ? a.fractionSize : b.fractionSize;

    for (int i = sum.fractionSize - 1; i >= 0; --i) {
        sumNumbers(sum.fraction[i], remainder, a.fraction[i], b.fraction[i]);
    }
    if (sum.fractionSize > 0 && sum.fraction[sum.fractionSize - 1] == 0) {
        --sum.fractionSize;
    }
    for (int i = 0; i < sum.integerSize; ++i) {
        sumNumbers(sum.integer[i], remainder, a.integer[i], b.integer[i]);
    }
    if (remainder > 0 && sum.integerSize < DATA_SIZE) {
        sum.integer[sum.integerSize++] = remainder;
    }

    return sum;
}


// вычитает целую и дробную части, игнорируя знак
LongNum absoluteSub(LongNum& a, LongNum& b) {
    LongNum sub;
    int index;
    bool less;
    int remainder = 0;
    bool aSign = a.sign;
    bool bSign = b.sign;

    a.sign = true;
    b.sign = true;
    less = isLess(a, b);
    a.sign = aSign;
    b.sign = bSign;

    LongNum& higher = less ? b : a;
    LongNum& lower = less ? a : b;
    sub.fractionSize = higher.fractionSize;

    while (sub.fractionSize > 0 && higher.fraction[sub.fractionSize - 1] == lower.fraction[sub.fractionSize - 1]) {
        sub.fraction[sub.fractionSize - 1] = 0;
        --sub.fractionSize;
    }
    for (int i = sub.fractionSize - 1; i >= 0; --i) {
        subNumbers(sub.fraction[i], remainder, a.fraction[i], b.fraction[i]);
        index = i - 1;
        while (index >= 0 && remainder == -1) {
            subNumbers(sub.fraction[index], remainder, a.fraction[index], 1);
            --index;
        }
        if (index < 0 && remainder == -1) {
            break;
        }
    }

    return sub;
}


// парсит строку, в которой должно находиться число LongNum, и при успехе преобразует строку в num
bool readLongNum(std::ifstream& inFile, LongNum& num) {
    std::string line;
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

        num.integerSize = 0;
        element = "";
        for (int i = integerLength - 1; i >= 0 && num.integerSize < DATA_SIZE; --i) {
            element += integerString[i];
            if (element.length() == 2 || i == 0) {
                element = reverse(element);
                num.integer[num.integerSize++] = std::stoi(element);
                element.clear();
            }
        }

        num.fractionSize = 0;
        element = "";
        for (int i = 0; i < fractionLength && num.fractionSize < DATA_SIZE; ++i) {
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

        return true;
    }
    return false;
}


// выводит число LongNum в читаемом виде в конец файла outFile
void writeLongNum(std::ofstream& outFile, LongNum& num) {
    if (!num.sign) {
        outFile << '-';
    }
    if (num.integerSize > 0) {
        for (int i = num.integerSize - 1; i >= 0; --i) {
            if (num.integer[i] < 10 && i < num.integerSize - 1) {
                outFile << '0';
            }
            outFile << static_cast<short>(num.integer[i]);
        }
    } else {
        outFile << '0';
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


bool isEqual(LongNum& a, LongNum& b) {
    if (a.sign == b.sign && a.integerSize == b.integerSize && a.fractionSize == b.fractionSize) {
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


bool isLess(LongNum& a, LongNum& b) {
    if (a.sign == b.sign) {
        LongNum& lower = a.sign ? a : b;
        LongNum& higher = a.sign ? b : a;
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


LongNum sumLongNum(LongNum& a, LongNum& b) {
    LongNum sum;

}


LongNum subLongNum(LongNum& a, LongNum& b) {
    // написать
}