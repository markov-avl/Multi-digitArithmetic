#include <iostream>


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


std::string invalidLongNum(unsigned int index) {
    return "Строка " + std::to_string(index) + " - неверно введено число LongNum (проверьте правильность синтаксиса " +
        "числа LongNum и убедитесь, что в начале и в конце строки не присутствуют лишние пробелы)";
}


std::string invalidSign(unsigned int index) {
    return "Строка " + std::to_string(index) + " - неверно введен арифметический знак (поддерживаются только знаки " +
        "\"+\" и \"-\", и также убедитесь, что в начале и в конце строки не присутствуют лишние пробелы)";
}


std::string dataOverflowed() {
    return "Произошло переполнение памяти под число, итоговое вычисление будет неверным";
}