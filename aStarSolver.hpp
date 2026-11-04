#ifndef ASTARSOLVER_HPP
#define ASTARSOLVER_HPP

#include <vector>
#include "gameState.hpp"
#include "solverUtils.hpp"

inline constexpr int MAX_AI_DIMENSION = 4;

struct AStarResult {
    bool success = false;
    std::vector<gameState*> path; // Path from start state to goal
    size_t nodesExpanded = 0;
};

// Solves the N-puzzle using A* search.
AStarResult solveAStar(const gameState &startState);

// Prints each step of the solution path from start to goal
void printSolutionPath(const AStarResult &result);

#endif // ASTARSOLVER_HPP