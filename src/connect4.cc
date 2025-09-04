#include <ncurses.h>
#include <locale.h>

char board[6][7];
const char p1 = 'X';
const char p2 = 'O';
bool turn = 0; // 0 = p1, 1 = p2

void init() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      board[i][j] = 'X';
    }
  }
}

void draw() {
  move(0, 0);
  turn ? printw("Player 2's turn:\n") : printw("Player 1's turn:\n");
  printw("\n┌───┬───┬───┬───┬───┬───┬───┐\n");
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      if (j != 0) {
        printw(" %c │", board[i][j]);
      } else {
        printw("│ %c │", board[i][j]);
      }
    }
    if (i != 5) {
      printw("\n├───┼───┼───┼───┼───┼───┼───┤\n");
    }
  }
  printw("\n└───┴───┴───┴───┴───┴───┴───┘\n");
}

void gameloop() {
  while (true) {
    draw();
    refresh();
    getch();
  }
}

int main() {
  setlocale(LC_ALL, ""); // Generate locales if not working
  initscr();
  noecho();

  init();
  gameloop();

	endwin();
  return 0;
}
