#include <iostream>

std::string fileNotFound(const std::string &path) {
    return "Невозможно найти входной файл по адресу: \"" + path + "\"";
}

std::string fileIsEmpty(const std::string &path) {
    return "Входной файл \"" + path + "\" пуст";
}

std::string successfullyCompleted(const std::string &inPath, const std::string &outPath) {
    return "Файл \"" + inPath + "\" успешно обработан\nРезультат можно просмотреть в файле \"" + outPath + "\"";
}

std::string unsuccessfullyCompleted(const std::string &inPath, const std::string &outPath) {
    return "В файле \"" + inPath + "\" были найдены ошибки\nОшибки можно просмотреть в файле \"" + outPath + "\"";
}