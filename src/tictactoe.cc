#include <ncurses.h>
#include <locale.h>

// TODO:
// have bigger boards
// make cursor thick
// optimise ugly code

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

// row1, row2, row3, col1, col2, col3, diagnegative, diagpositive
int p1mask[8] = {0};
int p2mask[8] = {0};

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
        printw(" %c │", board[i][j]);
      } else {
        printw(" %c", board[i][j]);
      }
    }
    if (i != 2) {
      printw("\n───┼───┼───\n");
    } else {
      printw("\n");
    }
  }
  move(y, x);
}

void update() {
  if (turn) {
    board[currY][currX] = p2;
    p2mask[currY + 3]++;
    p2mask[currX]++;
    if (currX == currY) {
      p2mask[6]++;
    }
    if (currX + currY == 2) {
      p2mask[7]++;
    }
  } else {
    board[currY][currX] = p1;
    p1mask[currY + 3]++;
    p1mask[currX]++;
    if (currX == currY) {
      p1mask[6]++;
    }
    if (currX + currY == 2) {
      p1mask[7]++;
    }
  }
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
        update();
        break;
    }
  }
}

int win() {
  for (int i = 0; i < 8; i++) {
    if (p1mask[i] == 3) {
      return 1;
    }
    if (p2mask[i] == 3) {
      return 2;
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        return 0;
      }
    }
  }

  return 3;
}

void gameloop() {
  while (true) {
    draw();
	  refresh();
    int result = win();
    if (result > 0) {
      move(10, 0);
      if (result == 1) {
        printw("Game Over! Player 1 wins! Press any key to exit.");
      } else if (result == 2) {
        printw("Game Over! Player 2 wins! Press any key to exit.");
      } else {
        printw("Game Over! It's a draw! Press any key to exit.");
      }
      refresh();
      getch();
      break;
    }
    select();
    turn = !turn;
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
