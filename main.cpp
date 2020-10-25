#include <iostream>
#include "constants.h"
#include "messages.h"
#include "process.h"


int main() {
    std::ifstream inFile(IN_PATH);
    std::ofstream outFile(OUT_PATH);
    bool isFileCorrect;

    if ( !inFile.is_open() ) {
        outFile << fileNotFound(IN_PATH);
        isFileCorrect = false;
    } else {
        isFileCorrect = processFile(inFile, outFile);
    }

    inFile.close();
    outFile.close();

    if (isFileCorrect) {
        std::cout << successfullyCompleted(IN_PATH, OUT_PATH);
    } else {
        std::cout << unsuccessfullyCompleted(IN_PATH, OUT_PATH);
    }

    return 0;
}