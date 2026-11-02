#ifndef ASTARSOLVER_HPP
#define ASTARSOLVER_HPP

#include <vector>
#include "gameState.hpp"
#include "solverUtils.hpp"

struct AStarResult {
    bool success = false;
    std::vector<gameState*> path; // Path from start state to goal
    size_t nodesExpanded = 0;
};

// Solves the N-puzzle using A* search.
// Returns an AStarResult containing the solution path and execution stats.
AStarResult solveAStar(const gameState &startState);

#endif // ASTARSOLVER_HPP