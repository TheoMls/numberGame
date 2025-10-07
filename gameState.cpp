#include <iostream>
#include<cmath>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>
#include "gameState.hpp"

gameState::gameState(int dimension) : dimension(dimension){
    int size = static_cast<int>(pow(dimension, 2));
    //fill board with random numbers
    board = std::vector(dimension, std::vector<int>(dimension,0));
    std::vector<int> board1d;
    for (size_t i = 0; i < size; i++){
        board1d.push_back(i);
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(board1d.begin(), board1d.end(), std::default_random_engine(seed));
    
    for (size_t i = 0; i < size; i++){
        int row = i / dimension;
        int column = i % dimension;
        board[row][column] = board1d[i];
    }
}

void gameState::print(){
    int maxNum = pow(dimension, 2) - 1;
    int width = static_cast<int>(std::to_string(maxNum).length()) + 1;
    for (const auto &row : board) {
        for (int val : row) {
            if (val == 0)
                std::cout << std::setw(width) << " ";
            else
                std::cout << std::setw(width) << val;
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}