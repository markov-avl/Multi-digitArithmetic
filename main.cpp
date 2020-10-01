#include <iostream>
#include <fstream>
#include "messages.h"
#include "constants.h"

bool process(std::ifstream &inFile, std::ofstream &outFile, const char* inPath) {
    int index = 0;
    if (!inFile.is_open()) {
        outFile << fileNotFound(inPath);
    } else {
        std::string line;
        int sum = 0;
        bool sign = true;

        while (!inFile.eof())
        {
            getline(inFile, line);
            if (index % 2 == 0) {
                if (sign) {
                    sum += stoi(line, nullptr);
                } else {
                    sum -= stoi(line, nullptr);
                }
            } else {
                sign = line[0] == '+';
            }
            index++;
        }

        if (index == 0) {
            outFile << fileIsEmpty(inPath);
        } else {
            outFile << sum;
        }
        return index > 0;
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
