#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include "gameState.hpp"
#include "solverUtils.hpp"
#include "aStarSolver.hpp"
#include "tests.hpp"

// Reusable benchmark runner for any board layout
bool benchmarkBoard(const gameState &testState, int numTries, const std::string &label) {
    std::cout << "Running " << numTries << " test(s) on " << label << "...\n";

    if (!testState.isSolvable()) {
        std::cout << "Error: " << label << " layout is NOT solvable!\n\n";
        return false;
    }

    double totalTimeMs = 0.0;
    int successfulRuns = 0;
    AStarResult lastResult;

    for (int run = 0; run < numTries; ++run) {
        auto startTimer = std::chrono::high_resolution_clock::now();
        lastResult = solveAStar(testState);
        auto endTimer = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = endTimer - startTimer;
        totalTimeMs += duration.count();

        std::cout << " - Run " << (run + 1) << "/" << numTries << ": ";
        printFormattedTime(duration.count());
        std::cout << " | Expanded: " << lastResult.nodesExpanded;

        if (lastResult.success) {
            std::cout << " | Path steps: " << (lastResult.path.size() - 1) << "\n";
            successfulRuns++;
        } else {
            std::cout << " | FAILED\n";
        }

        // Clean up heap memory for search path
        for (gameState* node : lastResult.path) {
            delete node;
        }
    }

    double avgTimeMs = totalTimeMs / numTries;
    std::cout << "Average " << label << " search completion: ";
    printFormattedTime(avgTimeMs);
    std::cout << "\n\n";

    return (successfulRuns == numTries);
}

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
    for (gameState* child : children)
        openSet.push(child);

    int minF = std::min(stateA.getMoves() + stateA.getScore(), 
                        stateC.getMoves() + stateC.getScore());

    for (gameState* child : children)
        minF = std::min(minF, child->getMoves() + child->getScore());

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

    for (gameState* child : children)
        delete child;

    // TEST 5: 3x3 solveAStar Benchmarking
    std::cout << "[TEST 5] Testing 3x3 solveAStar Engine...\n";
    std::vector<std::vector<int>> boardLayoutD = {
        {0, 8, 7},
        {5, 4, 6},
        {3, 2, 1}
    };
    gameState stateD(boardLayoutD);

    if (benchmarkBoard(stateD, 20, "3x3 board")) {
        std::cout << "   >>> TEST 5 PASSED! <<<\n\n";
        passedTests++;
    } else {
        std::cout << "   >>> TEST 5 FAILED! <<<\n\n";
        failedTests++;
    }

    // TEST 6: 4x4 solveAStar Benchmarking
    std::cout << "[TEST 6] Testing 4x4 solveAStar Engine...\n";
    std::vector<std::vector<int>> boardLayout4x4 = {
        { 6,  2,  3,  5},
        { 0,  1,  9, 13},
        {10, 15, 12,  7},
        { 8, 14, 11,  4}
    };
    gameState state4x4(boardLayout4x4);

    int numOfExamples4x4 = 5;
    if (benchmarkBoard(state4x4, numOfExamples4x4, "4x4 board")) {
        std::cout << "   >>> TEST 6 PASSED! <<<\n\n";
        passedTests++;
    } else {
        std::cout << "   >>> TEST 6 FAILED! <<<\n\n";
        failedTests++;
    }

    std::cout << "========================================\n";
    std::cout << "   ALL COMPONENT CHECKS COMPLETE!\n";
    std::cout << "   Passed: " << passedTests << " | Failed: " << failedTests << "\n";
    std::cout << "========================================\n";
}