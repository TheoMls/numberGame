#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <tuple>

class gameState {
private:
    std::vector<std::vector<int>> board;
    int dimension;

    int emptyTileColumn;
    int emptyTileRow;

    //heuristic score
    int score;

    gameState *parent = nullptr;

    // internal heuristic helpers
    void countOfPlace();
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

    int getDimension() const { return dimension; };
    //void setDimension(int n) { this->dimension = n; };

    int getEmptyTileColumn() const { return emptyTileColumn; };
    void setEmptyTileColumn(int y) { this->emptyTileColumn = y; };
    
    int getEmptyTileRow() const { return emptyTileRow; };
    void setEmptyTileRow(int x) { this->emptyTileRow = x; };

    gameState *getParent() const {return this->parent; };
    void setParent(gameState *parent) {this->parent = parent; };
    
    std::vector<gameState*> getChildren();
    
    int getScore() const {return score; };
    void setScore(int s) {this->score = s; };

    bool moveUp(), moveDown(), moveLeft(), moveRight();
    
    bool isGoal() const;

    std::size_t identifier() const;
    std::size_t hashCode() const;

    bool operator==(const gameState &other) const;
};

#endif //GAMESTATE_HPP