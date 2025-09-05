#include <ncurses.h>
#include <locale.h>

// TODO:
// have bigger boards
// make cursor thick
// optimise ugly code

char board[3][3];
const char P1_ = 'X';
const char P2_ = 'O';
bool turn = 0; // 0 = p1, 1 = p2
int x = 1;
int y = 1;
int width = 3;
int height = 1;

// row1, row2, row3, col1, col2, col3, diagnegative, diagpositive
int p1[8];
int p2[8];

void init() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }
  move(y * (height + 1) + height / 2 + 1, 
       x * (width + 1) + width / 2);
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
      printw("\n");
      for (int j = 0; j < 3; j++) {
        for (int k = 0; k < width; k++) {
          printw("─");
        }
        if (j != 2) {
          printw("┼");
        }
      }
      printw("\n");
    } else {
      printw("\n");
    }
  }
  move(y * (height + 1) + height / 2 + 1, 
       x * (width + 1) + width / 2);
}

void update() {
  if (turn) {
    board[y][x] = P2_;
    p2[y + 3]++;
    p2[x]++;
    if (x == y) {
      p2[6]++;
    }
    if (x + y == 2) {
      p2[7]++;
    }
  } else {
    board[y][x] = P1_;
    p1[y + 3]++;
    p1[x]++;
    if (x == y) {
      p1[6]++;
    }
    if (x + y == 2) {
      p1[7]++;
    }
  }
}

void select() {
  int ch = -1;
  while (ch != -2) {
	  ch = getch();
    switch(ch) {
      case 65:    // key up
        if (y > 0) {
          y--;
        }
        move(y * (height + 1) + height / 2 + 1, 
            x * (width + 1) + width / 2);
        break;
      case 66:    // key down
        if (y < 2) {
          y++;
        }
        move(y * (height + 1) + height / 2 + 1, 
            x * (width + 1) + width / 2);
        break;
      case 67:    // key right
        if (x < 2) {
          x++;
        }
        move(y * (height + 1) + height / 2 + 1, 
            x * (width + 1) + width / 2);
        break;
      case 68:    // key left
        if (x > 0) {
          x--;
        }
        move(y * (height + 1) + height / 2 + 1, 
            x * (width + 1) + width / 2);
        break;
      case '\n':  // return
        if (board[y][x] == ' ') ch = -2;
        update();
        break;
      case '1':
        width = 3;
        height = 1;
        draw();
        break;
      case '2':
        width = 5;
        height = 2;
        draw();
        break;
    }
  }
}

int win() {
  for (int i = 0; i < 8; i++) {
    if (p1[i] == 3) {
      return 1;
    }
    if (p2[i] == 3) {
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
