#include <iostream>
#include<cmath>
#include <iomanip>
#include <algorithm>
#include <random>
#include <tuple>
#include "gameState.hpp"
#include "inversionsCounter.hpp"
#include <string>

gameState::gameState(int dimension)
    : dimension(dimension), moves(0), score(0) {
    int size = static_cast<int>(pow(dimension, 2));
    
    // Generate new starting board until its solvable
    
    // Fill board with numbers
    board = std::vector(dimension, std::vector<int>(dimension,0));
    std::vector<int> board1d;
    for (int i = 0; i < size; i++)
        board1d.push_back(i);
    do{
        std::vector<int> board1d_solvable = board1d;
        
        // Shuffle the board
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(board1d_solvable.begin(), board1d_solvable.end(), g);
        for (int i = 0; i < size; i++){
            int row = i / dimension;
            int column = i % dimension;
            board[row][column] = board1d_solvable[i];
        }
    } while (!isSolvable());
    std::tuple<int, int> emptyTile = findEmptyTilePos();
    emptyTileRow = get<0>(emptyTile);
    emptyTileColumn = get<1>(emptyTile);

    countManhattanDistance();
}

gameState::gameState(std::vector<std::vector<int>> board)
    : board(board), moves(0), score(0) {
    dimension = board.size();
    std::tuple<int, int> emptyTile = findEmptyTilePos();
    emptyTileRow = get<0>(emptyTile);
    emptyTileColumn = get<1>(emptyTile);
    countManhattanDistance();
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
    countManhattanDistance();
    std::cout << "Score: " << score << std::endl;
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

bool gameState::isSolvable() const {
    std::vector<int> flatBoard = to1D();

    // Remove '0' so it doesn't affect the inversion count
    flatBoard.erase(std::remove(flatBoard.begin(), flatBoard.end(), 0), flatBoard.end());

    int inversions = inversionCount(flatBoard);

    if (dimension % 2 != 0) {
        return inversions % 2 == 0;
    } else {
        int emptyRowFromBottom = dimension - std::get<0>(findEmptyTilePos());
        return (inversions + emptyRowFromBottom - 1) % 2 == 0;
    }
}

std::tuple<int, int> gameState::findEmptyTilePos() const{
    std::tuple<int, int> emptyTile;
    for (int x = 0; x < dimension; x++){
        for (int y = 0; y < dimension; y++){
            if (board[x][y] == 0) {
                emptyTile = std::make_tuple(x,y);   
            }
        }
    }
    return emptyTile;
}

void gameState::setBoard(const std::vector<std::vector<int>> &board){
    this->board = board;
    dimension = board.size();
    std::tuple<int, int> emptyTile = findEmptyTilePos();
    setEmptyTileRow(get<0>(emptyTile));
    setEmptyTileColumn(get<1>(emptyTile));
}

std::vector<gameState*> gameState::getChildren(){
    std::vector<gameState*> children;

    //TODO: implement getChildren()

    return children;
}

bool gameState::moveUp() {
    if (emptyTileRow == 0) {
        return false;
    }
    std::swap(board[emptyTileRow][emptyTileColumn], board[emptyTileRow - 1][emptyTileColumn]);
    emptyTileRow--;
    return true;
}

bool gameState::moveDown() {
    if (emptyTileRow == dimension - 1) {
        return false;
    }
    std::swap(board[emptyTileRow][emptyTileColumn], board[emptyTileRow + 1][emptyTileColumn]);
    emptyTileRow++;
    return true;
}

bool gameState::moveLeft() {
    if (emptyTileColumn == 0) {
        return false;
    }
    std::swap(board[emptyTileRow][emptyTileColumn], board[emptyTileRow][emptyTileColumn - 1]);
    emptyTileColumn--;
    return true;
}

bool gameState::moveRight() {
    if (emptyTileColumn == dimension - 1) {
        return false;
    }
    std::swap(board[emptyTileRow][emptyTileColumn], board[emptyTileRow][emptyTileColumn + 1]);
    emptyTileColumn++;
    return true;
}

bool gameState::isGoal() const {
    std::vector<int> flat = to1D();
    int size = flat.size();

    for (int i = 0; i < size; ++i) {
        // Tile at index i must equal (i + 1) % size
        // Indices 0 to size-2 map to 1 to size-1
        // Index size-1 maps to 0 (the empty tile)
        if (flat[i] != (i + 1) % size) {
            return false;
        }
    }

    return true;
}

void gameState::countManhattanDistance()
{
    int totalDistance = 0;

    for (int r = 0; r < dimension; ++r){
        for (int c = 0; c < dimension; ++c){
            int val = board[r][c];

            if (val == 0) continue;

            int targetRow = (val - 1) / dimension;
            int targetCol = (val - 1) % dimension;

            totalDistance += std::abs(r - targetRow) + std::abs(c - targetCol);
        }
    }
    this->score = totalDistance;
}