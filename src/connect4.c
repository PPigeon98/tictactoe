#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

char board[6][7];
const char P1_ = 'X';
const char P2_ = 'O';
bool turn = 0; // 0 = p1, 1 = p2
int lowest[7] = {5, 5, 5, 5, 5, 5, 5};
int x = 3;

void init() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      board[i][j] = ' ';
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
  move(1, x * 4 + 2);
}

void selection() {
  int ch = -1;
  while (ch != -2) {
	  ch = getch();
    switch(ch) {
      case 67:    // key right
        if (x < 6) x++;
        move(1, x * 4 + 2);
        break;
      case 68:    // key left
        if (x > 0) x--;
        move(1, x * 4 + 2);
        break;
      case '\n':  // return
        if (board[lowest[x]][x] == ' ') ch = -2;
        turn ? (board[lowest[x]][x] = P2_) : (board[lowest[x]][x] = P1_);
        lowest[x]--;
        break;
      case '?':  // help
        move(0, 0);
        printw("Controls: Arrow keys to move, Enter to place, +/- to resize, = to toggle mode, ? for help, q to quit\n");
        move(1, x * 4 + 2);
        break;
      case 'q':  // exit
        endwin();
        exit(0);
        break;
    }
  }
}

int win() {
  for (int i = 0; i < 4; i++) {
    if ((board[lowest[x] + 1][i] == P1_ || board[lowest[x] + 1][i] == P2_) && board[lowest[x] + 1][i] == board[lowest[x] + 1][i + 1] && board[lowest[x] + 1][i] == board[lowest[x] + 1][i + 2] && board[lowest[x] + 1][i] == board[lowest[x] + 1][i + 3]) {
      return turn ? 1 : 2;
    }
  }
  
  for (int i = 0; i < 3; i++) {
    if ((board[i][x] == P1_ || board[i][x] == P2_) && board[i][x] == board[i + 1][x] && board[i][x] == board[i + 2][x] && board[i][x] == board[i + 3][x]) {
      return turn ? 1 : 2;
    }
  }

  for (int i = max(0, lowest[x] + 1 - 3); i <= min(2, lowest[x] + 1); i++) {
    int j = x - (lowest[x] + 1 - i);
    if (j >= 0 && j <= 3) {
      if ((board[i][j] == P1_ || board[i][j] == P2_) &&
          board[i][j] == board[i + 1][j + 1] &&
          board[i][j] == board[i + 2][j + 2] &&
          board[i][j] == board[i + 3][j + 3]) {
        return turn ? 1 : 2;
      }
    }
  }

  for (int i = max(0, lowest[x] + 1 - 3); i <= min(2, lowest[x] + 1); i++) {
    int j = x + (lowest[x] + 1 - i);
    if (j >= 3 && j <= 6) {
      if ((board[i][j] == P1_ || board[i][j] == P2_) &&
          board[i][j] == board[i + 1][j - 1] &&
          board[i][j] == board[i + 2][j - 2] &&
          board[i][j] == board[i + 3][j - 3]) {
        return turn ? 1 : 2;
      }
    }
  }

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
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
      move(0, 0);
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

    selection();
    turn = !turn;
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
