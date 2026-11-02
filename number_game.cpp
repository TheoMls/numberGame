#include <iostream>
#include <cctype>
#include <string>
#include <chrono>
#include "gameState.hpp"
#include "solverUtils.hpp"
#include "tests.hpp"
#include "aStarSolver.hpp"

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

        char dir = std::tolower(input[input.length() - 1]);

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
            case 'w': moved = state.moveDown(); break;
            case 's': moved = state.moveUp(); break;
            case 'a': moved = state.moveRight(); break;
            case 'd': moved = state.moveLeft(); break;
        }

        if (moved) {
            state.incrMoves();
            std::cout << "\nMoves: " << state.getMoves() << "\n";
        } else {
            std::cout << "Cannot move in that direction (boundary reached).\n";
        }
    }

    std::cout << "\nCongratulations! You solved the puzzle in " 
              << state.getMoves() << " moves!\n";
    state.print();
}

void handleAISolver(const gameState &startingBoard) {
    std::cout << "\nSolving board using A* search algorithm...\n";

    auto startTimer = std::chrono::high_resolution_clock::now();
    AStarResult result = solveAStar(startingBoard);
    auto endTimer = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = endTimer - startTimer;

    if (result.success) {
        std::cout << "Search completed in " << duration.count() << " ms.\n";
        printSolutionPath(result);

        // Memory Cleanup for solution path
        for (gameState* node : result.path) {
            delete node;
        }
    } else {
        std::cout << "\nFailed to find a solution path for this puzzle.\n";
    }
}

int main(int argc, char* argv[]) {
    // Run component tests if "test" argument is supplied
    if (argc > 1 && std::string(argv[1]) == "test") {
        runTests();
        return 0;
    }

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
            std::cout << "Invalid input. Using default dimension (3).\n";
            dimension = 3;
        }
    }

    gameState startingBoard = gameState(dimension);

    std::cout << "\nChoose Mode:\n";
    std::cout << "1. Play Manually (WASD)\n";
    std::cout << "2. Solve automatically with AI (A* Search)\n";
    std::cout << "Selection [default 1]: ";

    std::string modeChoice;
    std::getline(std::cin, modeChoice);

    if (!modeChoice.empty() && modeChoice[0] == '2') {
        handleAISolver(startingBoard);
    } else {
        handleUserMove(startingBoard);
    }

    return 0;
}