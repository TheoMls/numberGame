#include <iostream>
#include<cmath>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>
#include "gameState.hpp"
#include "inversionsCounter.hpp"

gameState::gameState(int dimension) : dimension(dimension){
    int size = static_cast<int>(pow(dimension, 2));
    // Generate new starting board until its solvable
    do{

        // Fill board with numbers
        board = std::vector(dimension, std::vector<int>(dimension,0));
        std::vector<int> board1d;
        for (size_t i = 0; i < size; i++){
            board1d.push_back(i);
        }

        // Shuffle the board
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(board1d.begin(), board1d.end(), std::default_random_engine(seed)); 
        for (size_t i = 0; i < size; i++){
            int row = i / dimension;
            int column = i % dimension;
            board[row][column] = board1d[i];
            if (board1d[i] == 0) {
                emptyTileRow = row;
                emptyTileColumn = column;    
            }
        }

    } while (!isSolvable());
}

gameState::gameState(std::vector<std::vector<int>> board): board(board){
    dimension = board.size();
    for (size_t x = 0; x < dimension; x++){
        for (size_t y = 0; y < dimension; y++){
            if (board[x][y] == 0) {
                emptyTileRow = x;
                emptyTileColumn = y;    
            }
        }
    }
}

void gameState::print(){
    int maxNum = pow(dimension, 2) - 1;
    int width = static_cast<int>(std::to_string(maxNum).length()) + 1;

    std::string newLine((width+2)*dimension-width/2, '-');
    std::cout<<std::endl;
    for (const auto &row : board) {
        for (int val : row) {
            if (val == 0)
                std::cout << std::setw(width+2) << ":";
            else
                std::cout << std::setw(width) << val << " :";
        }
        std::cout << "\b \n"<< newLine << "\n";
    }
    std::cout << std::endl;
}

std::vector<int> gameState::to1D() const{
    std::vector<int> board1D;
    board1D.reserve(dimension*dimension);
    for (auto &row : board){
        for (auto &&val : row){
            board1D.push_back(val);
        }
    }
    return board1D;
}

bool gameState::isSolvable() const{
    std::vector<int> flatBoard = to1D();
    int inversions;
    inversions = inversionCount(flatBoard);
    return inversions % 2 == 0;
}