#include <iostream>
#include <fstream>
#include <Windows.h>
#include "messages.h"
#include "constants.h"

bool process(std::ifstream &inFile, std::ofstream &outFile, const char* inPath) {
    if (!inFile.is_open()) {
        outFile << fileNotFound(inPath);
        return false;
    } else {
        std::string line;
        int index = 0;

        while (!inFile.eof())
        {
            getline(inFile, line);
            std::cout << line << std::endl;
            index++;
        }
        if (index == 0) {
            outFile << fileIsEmpty(inPath);
            return false;
        } else {
            return true;
        }
    }
}

int main() {
    std::ifstream inFile(IN_PATH);
    std::ofstream outFile(OUT_PATH);

    if (process(inFile, outFile, IN_PATH)) {
        std::cout << successfullyCompleted(IN_PATH, OUT_PATH);
    } else {
        std::cout << unsuccessfullyCompleted(IN_PATH, OUT_PATH);
    }

    inFile.close();
    outFile.close();

    return 0;
}
