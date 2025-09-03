#include <iostream>
#include <ncurses.h>
#include <locale.h>

// TODO:
// use unicode characters
// have bigger boards
// make cursor thick
// win/lose

char board[3][3];
const char p1 = 'X';
const char p2 = 'O';
bool turn = 0; // 0 = p1, 1 = p2
int x = 5;
int y = 3;
int start = 2;
int end = 6;
int endY = 5;
int size = 4;
int sizey = 2;
int currX = 1;
int currY = 1;

// From lsb:
// r1
// r2
// r3
// c1
// c2
// c3
// d
uint8_t p1mask = 0;
uint8_t p2mask = 0;

void init() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }
  move(y, x);
}

void draw() {
  move(0, 0);
  turn ? printw("Player 2's turn:\n") : printw("Player 1's turn:\n");
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (j != 2) {
        printw(" %c |", board[i][j]);
      } else {
        printw(" %c", board[i][j]);
      }
    }
    if (i != 2) {
      printw("\n---+---+---\n");
    } else {
      printw("\n");
    }
  }
  move(y, x);
}

void select() {
  int ch = -1;
  while (ch != -2) {
	  ch = getch();
    switch(ch) {
      case 65:    // key up
        if (y > start) {
          y -= sizey;
          currY--;
        }
        move(y, x);
        break;
      case 66:    // key down
        if (y < endY) {
          y += sizey;
          currY++;
        }
        move(y, x);
        break;
      case 67:    // key right
        if (x < end) {
          x += size;
          currX++;
        }
        move(y, x);
        break;
      case 68:    // key left
        if (x > start) {
          x -= size;
          currX--;
        }
        move(y, x);
        break;
      case '\n':  // return
        if (board[currY][currX] == ' ') ch = -2;
        break;
    }
  }
  turn ? board[currY][currX] = p2 : board[currY][currX] = p1;
}

void check() {

}

void gameloop() {
  while (true) {
    draw();
	  refresh();
    select();
    turn = !turn;
  }
}

int main() {
  initscr();
  noecho();
  curs_set(2);

  init();
  gameloop();

	endwin();
  return 0;
}
