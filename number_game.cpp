#include <iostream>
#include<cmath>
#include "gameState.hpp"

int main() {
  std::cout << "HELLO :)\n";
  gameState startingBoard = gameState(3);
  startingBoard.print();
  return 0;
}