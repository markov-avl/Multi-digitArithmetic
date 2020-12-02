#include <iostream>
#include "constants.h"


std::string fileNotFound(const std::string &path) {
    return "Невозможно найти входной файл по адресу: \"" + path + "\"";
}


std::string fileIsEmpty(const std::string &path) {
    return "Входной файл \"" + path + "\" пуст";
}


std::string invalidEndOfFile(const std::string &path) {
    return "Файл \"" + path + "\" неверно завершен (последней строкой должно быть число)";
}


std::string successfullyCompleted(const std::string &inPath, const std::string &outPath) {
    return "Файл \"" + inPath + "\" успешно обработан\nРезультат можно просмотреть в файле \"" + outPath + "\"";
}


std::string unsuccessfullyCompleted(const std::string &inPath, const std::string &outPath) {
    return "В файле \"" + inPath + "\" были найдены ошибки\nОшибки можно просмотреть в файле \"" + outPath + "\"";
}


std::string unparsable(unsigned int index) {
    return "Строка " + std::to_string(index) + " - неверно введено число (проверьте правильность синтаксиса)";
}


std::string minusZero(unsigned int index) {
    return "Строка " + std::to_string(index) + " - ноль не может быть со знаком минус";
}


std::string integerOverflowed(unsigned int index) {
    return "Строка " + std::to_string(index) + " - целая часть числа переполнена (максимум " +
        std::to_string(DATA_SIZE * 2) + ")";
}


std::string fractionOverflowed(unsigned int index) {
    return "Строка " + std::to_string(index) + " - дробная часть числа переполнена (максимум " +
           std::to_string(DATA_SIZE * 2) + ")";
}


std::string integerAndFractionOverflowed(unsigned int index) {
    return "Строка " + std::to_string(index) + " - целая и дробная части числа переполнены (максимум " +
           std::to_string(DATA_SIZE * 2) + ")";
}


std::string operationIntegerOverflowed(unsigned int index) {
    return "Строка " + std::to_string(index) + " - произошло переполнение памяти под целую часть нового числа во " +
        "время выполнения операции";
}


std::string operationFractionOverflowed(unsigned int index) {
    return "Строка " + std::to_string(index) + " - произошло переполнение памяти под дробную часть нового числа во " +
           "время выполнения операции";
}


std::string invalidSign(unsigned int index) {
    return "Строка " + std::to_string(index) + " - неверно введен арифметический знак (поддерживаются только знаки " +
        "\"+\" и \"-\", и также убедитесь, что в начале и в конце строки не присутствуют лишние пробелы)";
}