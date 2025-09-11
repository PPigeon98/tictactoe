#include <ncurses.h>
#include <locale.h>

void init() {

}

void draw() {
  move(0, 0);

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
