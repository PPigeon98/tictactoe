#include <ncurses.h>
#include <locale.h>

int score = 0;
int gamewidth = 8;
int gameheight = 8;

void init() {

}

void draw() {
  move(0, 0);
  printw("Score = %d", score);
  for (int i = 0; i < gameheight; i++) {
    for (int j = 0; j < gamewidth; j++) {
      if (i == gamewidth - 1) {
        printw(" ");
      }
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
