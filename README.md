# N-Puzzle Solver (C++)

A modular, high-performance C++ implementation of the classic **N-Puzzle** game (8-puzzle, 15-puzzle, etc.) featuring interactive manual gameplay and an optimal **A\* Search Algorithm** solver.

## Features

- **Custom Dimension Boards:** Play standard 3x3 (8-puzzle) or higher-dimensional NxN sliding tile puzzles.
- **Solvability Validation:** Generates only solvable starting boards by counting inversions using an $O(N \log N)$ Merge-Sort counting algorithm.
- **A\* Search Engine:** Uses Manhattan Distance heuristic combined with step costs ($g(n) + h(n)$) to compute minimal solution paths.
- **Optimal Memory Management:** Tracks dynamic heap allocations to clean up dead-end search nodes while preserving solution paths.
- **Interactive Gameplay & Animation:** Play manually with WASD keys or watch the AI solve the puzzle step-by-step with animated move transitions.
- **Automated Test Suite:** Built-in verification testing for hashing, pointer equality, child expansion, heap ordering, and A* search correctness.

---

## Code Architecture

The codebase is structured around a clear separation of concerns:

- `gameState.hpp / .cpp`: Defines the board data structure, move operations, goal evaluation, Manhattan distance heuristic, and board state hashing.
- `inversionsCounter.hpp`: Implements an efficient Merge-Sort algorithm to calculate inversion counts and verify board solvability in $O(N \log N)$ time.
- `solverUtils.hpp`: Contains custom functors (`gameStatePtrHash`, `gameStatePtrEqual`, `CompareAStar`) for `std::unordered_set` and `std::priority_queue`.
- `aStarSolver.hpp / .cpp`: Implements the core A* search engine and solution path visualization utilities.
- `tests.hpp / .cpp`: Provides automated component test suites to verify algorithmic correctness.
- `number_game.cpp`: Entry point managing user interaction, CLI arguments, and mode selection.

---

## Compilation

Requires a C++17 compliant compiler (`g++`, `clang++`, or `MinGW`).

### Using `mingw32-make` (Windows):
```cmd
mingw32-make
```

### Using `g++` directly:
```bash
g++ -std=c++17 number_game.cpp gameState.cpp tests.cpp aStarSolver.cpp -o number_game.exe
```

---

## Usage

### 1. Interactive Mode
Run the executable directly to launch manual play or AI solver selection:
```cmd
.\number_game.exe
```

### 2. Component Testing Mode
Pass the `test` flag to execute all automated component checks:
```cmd
.\number_game.exe test
```

---

## Controls (Manual Play)

- **W**: Move tile Up (vacant slot moves Down)
- **S**: Move tile Down (vacant slot moves Up)
- **A**: Move tile Left (vacant slot moves Right)
- **D**: Move tile Right (vacant slot moves Left)