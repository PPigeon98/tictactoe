#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>

char board[8][8];
int x = 0;
int y = 0;
bool turn = 0; // 0 = p1, 1 = p2

void init() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = ' ';
    }
  }
}

void draw() {
  move(0, 0);
  turn ? printw("Player 2's turn:\n") : printw("Player 1's turn:\n");
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

    }
  }
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
  curs_set(2);
  noecho();

  init();
  gameloop();

	endwin();
  return 0;
}
