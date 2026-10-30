#include <iostream>
#include <string>
#include "gameState.hpp"

int main() {
    std::cout << "HELLO :)\nGive dimension [default 3]: ";
    std::string input;
    std::getline(std::cin, input);

    int dimension = 3;
    if (!input.empty()) {
        try {
            dimension = std::stoi(input);
            if (dimension < 2)
                throw std::exception();
        } catch (...) {
            std::cout << "Invalid input. Using default dimension 3.\n";
            dimension = 3;
        }
    }

    gameState startingBoard = gameState(dimension);
    startingBoard.print();
    return 0;
}