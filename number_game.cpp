#include <iostream>
#include <cctype>
#include <string>
#include "gameState.hpp"

char getValidDirection() {
    std::string input;

    while (true) {
        std::cout << "Enter direction (W = Up, A = Left, S = Down, D = Right): ";
        std::getline(std::cin, input);

        // Trim leading and trailing whitespace
        size_t start = input.find_first_not_of(" \t\r\n");
        if (start != std::string::npos) {
            size_t end = input.find_last_not_of(" \t\r\n");
            input = input.substr(start, end - start + 1);
        } else {
            input.clear();
        }

        // Reject empty input
        if (input.empty()) {
            std::cout << "Invalid input: Empty line/Enter is not allowed.\n";
            continue;
        }

        // Reject multi-character input (e.g. "wa", "up")
        if (input.length() > 1) {
            std::cout << "Invalid input: Please enter a single character.\n";
            continue;
        }

        char dir = std::tolower(input[0]);

        // Validate WASD keys
        if (dir == 'w' || dir == 'a' || dir == 's' || dir == 'd') {
            return dir;
        }

        std::cout << "Invalid direction! Use W, A, S, or D.\n";
    }
}

void handleUserMove(gameState &state) {
    while (!state.isGoal()) {
        state.print();
        char move = getValidDirection();

        bool moved = false;
        switch (move) {
            case 'w': moved = state.moveUp(); break;
            case 's': moved = state.moveDown(); break;
            case 'a': moved = state.moveLeft(); break;
            case 'd': moved = state.moveRight(); break;
        }

        if (!moved) {
            std::cout << "Cannot move in that direction (boundary reached).\n";
        }
    }

    std::cout << "\nCongratulations! You solved the puzzle!\n";
    state.print();
}

int main() {
    std::cout << "HELLO :)\nGive dimension [default 3]: ";
    std::string userDim;
    std::getline(std::cin, userDim);

    int dimension = 3;
    if (!userDim.empty()) {
        try {
            dimension = std::stoi(userDim);
            if (dimension < 2)
                throw std::exception();
        } catch (...) {
            std::cout << "Invalid input. Using default dimension 3.\n";
            dimension = 3;
        }
    }

    gameState startingBoard = gameState(dimension);
    
    handleUserMove(startingBoard);

    return 0;
}