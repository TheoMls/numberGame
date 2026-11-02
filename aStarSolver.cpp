#include <iostream>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <thread>
#include <chrono>
#include "aStarSolver.hpp"

AStarResult solveAStar(const gameState &startState) {
    AStarResult result;

    // If the starting board is already the goal
    if (startState.isGoal()) {
        result.success = true;
        result.path.push_back(new gameState(startState));
        return result;
    }

    std::priority_queue<gameState*, std::vector<gameState*>, CompareAStar> nodesToExplore;
    std::unordered_set<gameState*, gameStatePtrHash, gameStatePtrEqual> visited;

    // Track ALL dynamically allocated nodes to guarantee clean deletion on exit
    std::vector<gameState*> allNodes;

    // Allocate root node on heap so all path links remain valid during reconstruction
    gameState* root = new gameState(startState);
    allNodes.push_back(root);
    nodesToExplore.push(root);

    // Core Search Loop
    while (!nodesToExplore.empty()) {
        gameState* current = nodesToExplore.top();
        nodesToExplore.pop();

        // Skip node if it was already processed via an earlier popped duplicate
        if (visited.find(current) != visited.end())
            continue;

        // Insert node into visited set
        visited.insert(current);
        result.nodesExpanded++;

        // Goal check
        if (current->isGoal()) {
            result.success = true;

            // Reconstruct path by following parent pointers backward
            gameState* currPathNode = current;
            while (currPathNode != nullptr) {
                result.path.push_back(currPathNode);
                currPathNode = currPathNode->getParent();
            }

            // Reverse path
            std::reverse(result.path.begin(), result.path.end());

            // Clean up nodes created during search that were not part of the solution path
            std::unordered_set<gameState*> solutionSet(result.path.begin(), result.path.end());
            for (gameState* node : allNodes) {
                if (solutionSet.find(node) == solutionSet.end())
                    delete node;
            }

            return result;
        }

        // Explore children
        std::vector<gameState*> children = current->getChildren();
        for (gameState* child : children) {
            // Track allocated pointer for cleanup
            allNodes.push_back(child);

            // If child state was already evaluated, skip pushing to nodesToExplore
            if (visited.find(child) != visited.end())
                continue;

            nodesToExplore.push(child);
        }
    }

    // Search failed (unsolvable state or empty nodesToExplore)
    result.success = false;

    // Clean up all allocated memory
    for (gameState* node : allNodes)
        delete node;

    return result;
}

void printSolutionPath(const AStarResult &result) {
    if (!result.success) {
        std::cout << "\nNo solution path to display.\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "         SOLUTION PATH STEPS            \n";
    std::cout << "========================================\n";
    std::cout << "Total Moves Required: " << result.path.size() - 1 << "\n";
    std::cout << "Total Nodes Expanded: " << result.nodesExpanded << "\n";

    for (size_t i = 0; i < result.path.size(); ++i) {
        if (i == 0) {
            std::cout << "\n--- Step 0: Initial Board ---";
        } else {
            std::cout << "\n--- Step " << i << " (Move " << result.path[i]->getMoves() << ") ---";
        }
        
        result.path[i]->print();

        // Pause for 1 second between moves (skip pause after final goal state)
        if (i + 1 < result.path.size()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
    }

    std::cout << "========================================\n";
    std::cout << "             PUZZLE SOLVED!             \n";
    std::cout << "========================================\n";
}