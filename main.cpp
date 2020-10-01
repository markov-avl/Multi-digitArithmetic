#include <iostream>
#include "parse.h"
#include "messages.h"
#include "constants.h"


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