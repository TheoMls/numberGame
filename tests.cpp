#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "gameState.hpp"
#include "solverUtils.hpp"
#include "aStarSolver.hpp"
#include "tests.hpp"

void runTests() {
    int passedTests = 0;
    int failedTests = 0;

    std::cout << "========================================\n";
    std::cout << "   VERIFYING A* SEARCH COMPONENTS\n";
    std::cout << "========================================\n\n";

    // Setup Test Boards
    std::vector<std::vector<int>> boardLayoutA = {
        {1, 2, 3},
        {4, 0, 6},
        {7, 5, 8}
    };

    std::vector<std::vector<int>> boardLayoutB = {
        {1, 2, 3},
        {4, 0, 6},
        {7, 5, 8}
    };

    std::vector<std::vector<int>> boardLayoutC = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    gameState stateA(boardLayoutA);
    gameState stateB(boardLayoutB);
    gameState stateC(boardLayoutC);

    // TEST 1: operator== & hashCode()
    std::cout << "[TEST 1] Testing Equality and Hash Codes...\n";
    bool eqAB = (stateA == stateB);
    bool eqAC = (stateA == stateC);
    
    std::cout << " - stateA == stateB (Expected: 1): " << eqAB << "\n";
    std::cout << " - stateA == stateC (Expected: 0): " << eqAC << "\n";
    std::cout << " - stateA Hash: " << stateA.hashCode() << "\n";
    std::cout << " - stateB Hash: " << stateB.hashCode() << "\n";
    std::cout << " - stateC Hash: " << stateC.hashCode() << "\n";

    if (eqAB && !eqAC && (stateA.hashCode() == stateB.hashCode())) {
        std::cout << "   >>> TEST 1 PASSED! <<<\n\n";
        passedTests++;
    } else {
        std::cout << "   >>> TEST 1 FAILED! <<<\n\n";
        failedTests++;
    }

    // TEST 2: std::unordered_set Lookup
    std::cout << "[TEST 2] Testing Set Deduplication (gameStatePtrHash & Equal)...\n";
    std::unordered_set<gameState*, gameStatePtrHash, gameStatePtrEqual> closedSet;
    closedSet.insert(&stateA);

    bool foundB = (closedSet.find(&stateB) != closedSet.end());
    bool foundC = (closedSet.find(&stateC) != closedSet.end());

    std::cout << " - Found stateB via stateA pointer in set? (Expected: 1): " << foundB << "\n";
    std::cout << " - Found stateC in set? (Expected: 0): " << foundC << "\n";

    if (foundB && !foundC) {
        std::cout << "   >>> TEST 2 PASSED! <<<\n\n";
        passedTests++;
    } else {
        std::cout << "   >>> TEST 2 FAILED! <<<\n\n";
        failedTests++;
    }

    // TEST 3: getChildren()
    std::cout << "[TEST 3] Testing getChildren()...\n";
    std::vector<gameState*> children = stateA.getChildren();
    std::cout << " - Generated children count (Expected: 4): " << children.size() << "\n";

    bool childrenValid = (children.size() == 4);
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i]->getParent() != &stateA) childrenValid = false;
        if (children[i]->getMoves() != 1) childrenValid = false;
    }

    if (childrenValid) {
        std::cout << "   >>> TEST 3 PASSED! <<<\n\n";
        passedTests++;
    } else {
        std::cout << "   >>> TEST 3 FAILED! <<<\n\n";
        failedTests++;
    }

    // TEST 4: CompareAStar Min-Heap Order
    std::cout << "[TEST 4] Testing CompareAStar Priority Queue Ordering...\n";
    std::priority_queue<gameState*, std::vector<gameState*>, CompareAStar> openSet;

    stateC.setMoves(10);

    openSet.push(&stateC);
    openSet.push(&stateA);
    for (gameState* child : children) {
        openSet.push(child);
    }

    int minF = std::min(stateA.getMoves() + stateA.getScore(), 
                        stateC.getMoves() + stateC.getScore());

    for (gameState* child : children) {
        minF = std::min(minF, child->getMoves() + child->getScore());
    }

    gameState* topNode = openSet.top();
    int topF = topNode->getMoves() + topNode->getScore();

    bool topIsMinimal = (topF == minF);

    std::cout << " - Expected min f(n): " << minF << "\n";
    std::cout << " - Top node f(n): " << topF << "\n";
    std::cout << " - Top node is lowest f(n) state? (Expected: YES): " 
              << (topIsMinimal ? "YES" : "NO") << "\n";

    if (topIsMinimal) {
        std::cout << "   >>> TEST 4 PASSED! <<<\n\n";
        passedTests++;
    } else {
        std::cout << "   >>> TEST 4 FAILED! <<<\n\n";
        failedTests++;
    }

    for (gameState* child : children) {
        delete child;
    }

    // TEST 5: Full solveAStar Engine Execution
    std::cout << "[TEST 5] Testing solveAStar Engine...\n";
    AStarResult solverResult = solveAStar(stateA);

    bool solverSuccess = solverResult.success && 
                         !solverResult.path.empty() && 
                         solverResult.path.back()->isGoal();

    std::cout << " - Search returned success? (Expected: 1): " << solverResult.success << "\n";
    std::cout << " - Total path steps: " << solverResult.path.size() << "\n";
    std::cout << " - Nodes expanded: " << solverResult.nodesExpanded << "\n";
    std::cout << " - Final state is goal? (Expected: 1): " 
              << (solverResult.path.empty() ? 0 : solverResult.path.back()->isGoal()) << "\n";

    if (solverSuccess) {
        std::cout << "   >>> TEST 5 PASSED! <<<\n\n";
        passedTests++;
    } else {
        std::cout << "   >>> TEST 5 FAILED! <<<\n\n";
        failedTests++;
    }

    // Clean up heap memory allocated by solver test
    for (gameState* node : solverResult.path) {
        delete node;
    }

    std::cout << "========================================\n";
    std::cout << "   ALL COMPONENT CHECKS COMPLETE!\n";
    std::cout << "   Passed: " << passedTests << " | Failed: " << failedTests << "\n";
    std::cout << "========================================\n";
}