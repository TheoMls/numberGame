#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>

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
    gameState(std::vector<std::vector<int>> board): board(board){};

    void print();

    std::vector<int> to1D() const;
    bool isSolvable() const;
    
    const std::vector<std::vector<int>> &getBoard() const;
    void setBoard(const std::vector<std::vector<int>> &board);

    int getDimension() const;
    void setDimension(int n);

    int getEmptyTileColumn() const;
    void setEmptyTileColumn(int y);
    
    int getEmptyTileRow() const;
    void setEmptyTileRow(int x);

    gameState *getParent() const;
    void setParent(gameState *parent);

    int getScore() const;
    void setScore(int s);

    std::vector<gameState*> getChildren() const;

    bool moveUp(), moveDown(), moveLeft(), moveRight();
    
    bool isGoal() const;

    std::size_t identifier() const;
    std::size_t hashCode() const;

    bool operator==(const gameState &other) const;
};

#endif //GAMESTATE_HPP