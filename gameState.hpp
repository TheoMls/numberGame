#ifndef GAMESTATE
#define GAMESTATE

#include <vector>

class gameState {
private:
    std::vector<std::vector<int>> board;
    int dimension;

    int emptyTileColumn;
    int emptyTileRow;

    //heuristic score
    int score;

    gameState *father = nullptr;

    void countOfPlace();
    void countManhattanDistance();
    void evaluate(int &heuristic);
public:
    gameState(int dimension);
    gameState(std::vector<std::vector<int>> board): board(board){};

    void print();
    
    const std::vector<std::vector<int>> getBoard() const;
    void setBoard(std::vector<std::vector<int>> &board);

    int getDimension();
    void setDimension();

    int getEmptyTileColumn();
    void setEmptyTileColumn();
    
    int getEmptyTileRow();
    void setEmptyTileRow();

    gameState *getParent() const;
    void setParent(gameState *parent);

    int getScore();
    void setScore();

    std::vector<gameState*> getChildren() const;

    bool moveUp(), moveDown(), moveLeft(), moveRight();
    
    bool isGoal();

    std::size_t identifier() const;
    std::size_t hashCode() const;

    bool operator==(const gameState &other) const;
};

#endif //GAMESTATE