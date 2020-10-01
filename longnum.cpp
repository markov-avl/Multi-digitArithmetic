#include "longnum.h"


// приводит число LongNum к виду "+0", а также избавляется от мусора в массивах
LongNum getLongNum() {
    LongNum num;
    num.sign = true;
    for (int i = 0; i < DATA_SIZE; i++) {
        num.integer[i] = 0;
        num.fraction[i] = 0;
    }
    return num;
}


// парсит строку, в которой должно находиться число LongNum, и при успехе преобразует строку в num
bool readLongNum(std::string &line, LongNum &num) {
    // написать
}


// выводит число LongNum в читаемом виде в конец файла outFile
void writeLongNum(std::ofstream &outFile, LongNum &sum) {
    // написать
}