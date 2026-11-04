# N-Puzzle Solver (C++)

A modular, high-performance C++ implementation of the classic **N-Puzzle** game (8-puzzle, 15-puzzle, etc.) featuring interactive manual gameplay and an optimal **A* Search Algorithm*\* solver.

## Features

* **Custom Dimension Boards:** Play standard 3x3 (8-puzzle) or higher-dimensional NxN sliding tile puzzles.

* **Solvability Validation:** Generates only solvable starting boards by counting inversions using an $O(N \log N)$ Merge-Sort counting algorithm.

* **Advanced A* Search Engine:*\* Uses the **Manhattan Distance** combined with the **Linear Conflicts** heuristic to drastically prune the search space and compute minimal solution paths efficiently.

* **Search Optimizations:** Implements reverse-move pruning during node expansion and caps AI execution at 4x4 grids to guarantee safe memory bounds.

* **Optimal Memory Management:** Tracks dynamic heap allocations to clean up dead-end search nodes while preserving solution paths.

* **Interactive Gameplay & Animation:** Play manually with WASD keys or watch the AI solve the puzzle step-by-step with animated move transitions. Features dynamic execution time formatting.

* **Automated Benchmarking Suite:** Built-in verification testing for hashing, pointer equality, child expansion, heap ordering, and multi-run A\* benchmarking for 3x3 and 4x4 boards.

## Code Architecture

The codebase is structured around a clear separation of concerns:

* `gameState.hpp / .cpp`: Defines the board data structure, move operations, goal evaluation, reverse-move pruning, and calculates the Manhattan Distance + Linear Conflicts heuristic.

* `inversionsCounter.hpp`: Implements an efficient Merge-Sort algorithm to calculate inversion counts and verify board solvability in $O(N \log N)$ time.

* `solverUtils.hpp`: Contains time-formatting utilities and custom functors (`gameStatePtrHash`, `gameStatePtrEqual`, `CompareAStar`) for `std::unordered_set` and `std::priority_queue`.

* `aStarSolver.hpp / .cpp`: Implements the core A\* search engine, memory cleanup, and solution path visualization utilities.

* `tests.hpp / .cpp`: Provides automated component test suites and an average-time benchmarking runner.

* `number_game.cpp`: Entry point managing user interaction, CLI arguments, gameplay timer loops, and mode selection.

## Compilation

Requires a C++17 compliant compiler (`g++`, `clang++`, or `MinGW`).

### Using `mingw32-make` (Windows):

```
mingw32-make

```

### Using `g++` directly:

```
g++ -std=c++17 -Wall -Wextra -g3 number_game.cpp gameState.cpp tests.cpp aStarSolver.cpp -o number_game.exe

```

## Usage

### Interactive Mode

Run the executable directly to launch manual play or AI solver selection:

```
.\number_game.exe

```

### Component Testing & Benchmarking Mode

Pass the `test` flag to execute all automated component checks and A\* performance benchmarks:

```
.\number_game.exe test

```

## Controls (Manual Play)

* **W**: Move tile Up (vacant slot moves Down)

* **S**: Move tile Down (vacant slot moves Up)

* **A**: Move tile Left (vacant slot moves Right)

* **D**: Move tile Right (vacant slot moves Left)