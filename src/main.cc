#include <iostream>

char board[3][3];
const char p1 = 'X';
const char p2 = 'O';
bool turn = 0; // 0 = p1, 1 = p2

void init() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }
}

void draw() {
  std::cout << "Player " << (turn ? "2" : "1") << "'s turn:\n";
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (j != 2) {
        std::cout << ' ' << board[i][j] << " │";
      } else {
        std::cout << ' ' << board[i][j];
      }
    }
    if (i != 2) {
      std::cout << "\n───┼───┼───\n";
    } else {
      std::cout << '\n';
    }
  }
}

int main() {
  init();
  draw();

  return 0;
}
