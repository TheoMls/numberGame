#ifndef SOLVERUTILS_HPP
#define SOLVERUTILS_HPP

#include "gameState.hpp"

inline void printFormattedTime(double ms) {
    if (ms >= 1000.0)
        std::cout << (ms / 1000.0) << " s";
    else
        std::cout << ms << " ms";
}


// Struct that gets a gameState hash from its pointer (needed for unordered_set)
struct gameStatePtrHash {
    std::size_t operator()(const gameState *state) const {
        return state ? state->hashCode() : 0;
    }
};

// Struct that compares two gameState pointers by dereferencing them (needed for unordered_set)
struct gameStatePtrEqual {
    bool operator()(const gameState *a, const gameState *b) const {
        if (!a || !b) return a == b;
        return *a == *b;
    }
};

// Min-Heap priority queue comparator for A* search
struct CompareAStar {
    bool operator()(const gameState *a, const gameState *b) const {
        if (!a || !b) return a < b;

        int fA = a->getMoves() + a->getScore();
        int fB = b->getMoves() + b->getScore();

        // Min-Heap: if fA > fB, 'a' has LOWER priority than 'b'
        if (fA != fB)
            return fA > fB;

        // Prefer nodes with lower heuristic score
        return a->getScore() > b->getScore();
    }
};

#endif // SOLVERUTILS_HPP