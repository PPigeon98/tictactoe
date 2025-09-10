#include <ncurses.h>
#include <locale.h>

char board[9][9];

void init() {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      board = ' ';
    }
  }
}

void draw() {
  move(0, 0);
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {

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
  noecho();

  init();
  gameloop();

	endwin();
  return 0;
}
