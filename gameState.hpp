#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <tuple>
#include <cstddef>

class gameState {
private:
    std::vector<std::vector<int>> board;
    int dimension;

    int emptyTileColumn;
    int emptyTileRow;

    int moves = 0;
    int score = 0;

    gameState *parent = nullptr;

    void countManhattanDistance();
    void evaluate(int &heuristic);

public:
    gameState(int dimension);
    gameState(std::vector<std::vector<int>> board);

    void print();

    std::vector<int> to1D() const;
    bool isSolvable() const;

    std::tuple<int, int> findEmptyTilePos() const;
    
    const std::vector<std::vector<int>> &getBoard() const { return board; };
    void setBoard(const std::vector<std::vector<int>> &board);

    int incrMoves() { return ++moves; }
    void setMoves(int m) { this->moves = m; }
    int getMoves() const { return moves; }

    int getDimension() const { return dimension; };

    int getEmptyTileColumn() const { return emptyTileColumn; };
    void setEmptyTileColumn(int y) { this->emptyTileColumn = y; };
    
    int getEmptyTileRow() const { return emptyTileRow; };
    void setEmptyTileRow(int x) { this->emptyTileRow = x; };

    gameState *getParent() const { return this->parent; };
    void setParent(gameState *parent) { this->parent = parent; };
    
    std::vector<gameState*> getChildren();
    
    int getScore() const { return score; };
    void setScore(int s) { this->score = s; };

    bool moveUp(), moveDown(), moveLeft(), moveRight();
    
    bool isGoal() const;
        
    bool operator==(const gameState &other) const;
    std::size_t hashCode() const; 
};

// Struct that gets a gametState hash from its pointer (needed for unordered_set) 
struct gameStatePtrHash {
    std::size_t operator()(const gameState *state) const {
        return state ? state->hashCode() : 0;
    }
};

// Struct that compares to gameState pointers by dereferencing them (needed for unordered_set) 
struct gameStatePtrEqual {
    bool operator()(const gameState *a, const gameState *b) const {
        if (!a || !b) return a == b;
        return *a == *b;
    }
};

#endif //GAMESTATE_HPP