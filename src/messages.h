#include <iostream>

std::string fileNotFound(const std::string &path);
std::string fileIsEmpty(const std::string &path);
std::string invalidEndOfFile(const std::string &path);
std::string successfullyCompleted(const std::string &inPath, const std::string &outPath);
std::string unsuccessfullyCompleted(const std::string &inPath, const std::string &outPath);
std::string unparsable(unsigned int index);
std::string minusZero(unsigned int index);
std::string integerOverflowed(unsigned int index);
std::string fractionOverflowed(unsigned int index);
std::string integerAndFractionOverflowed(unsigned int index);
std::string operationIntegerOverflowed(unsigned int index);
std::string operationFractionOverflowed(unsigned int index);
std::string invalidSign(unsigned int index);