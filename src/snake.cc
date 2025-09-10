#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <queue>

enum Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT
} direction;

int score = 0;
int gamewidth = 16;
int gameheight = 8;
int x = 0;
int y = 0;

std::queue<std::pair<int, int>> snake;
std::pair<int, int> food;

void init() {
  x = gamewidth / 2;
  y = gameheight / 2;
}

void draw() {
  move(0, 0);
  printw("Score = %d\n", score);
  for (int i = 0; i < gameheight + 2; i++) {
    for (int j = 0; j < gamewidth + 2; j++) {
      if (i == 0) {
        if (j == 0) {
          printw("╔");
        } else if (j == gamewidth + 1) {
          printw("╗");
        } else {
          printw("═");
        }
      } else if (i == gameheight + 1) {
        if (j == 0) {
          printw("╚");
        } else if (j == gamewidth + 1) {
          printw("╝");
        } else {
          printw("═");
        }
      } else {
        if (j == 0 || j == gamewidth + 1) {
          printw("║");
        } else {
          printw(" ");
        }
      }
    }
    printw("\n");
  }
  printw("%d", direction);
}

void selection() {
  int ch = -1;
  while (ch != -2) {
	  ch = getch();
    switch(ch) {
      case 67:    // key right
        direction = static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
        ch = -2;
        break;
      case 68:    // key left
        direction = static_cast<Direction>((static_cast<int>(direction) + 3) % 4);
        ch = -2;
        break;
      case 'q':  // exit
        endwin();
        exit(0);
        break;
    }
  }
}

void moving() {

}

void gameloop() {
  while (true) {
    draw();
    refresh();
    selection();
  }
}

int main() {
  setlocale(LC_ALL, ""); // Generate locales if not working
  initscr();
  curs_set(0);
  noecho();

  init();
  gameloop();

	endwin();
  return 0;
}
