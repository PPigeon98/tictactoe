#include <ncurses.h>
#include <locale.h>

char board[3][3];
const char P1_ = 'X';
const char P2_ = 'O';
bool turn = 0; // 0 = p1, 1 = p2
int x = 1;
int y = 1;
int width = 3;
int height = 1;

void init() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }
  move(y * (height + 1) + height / 2 + 1, x * (width + 1) + width / 2);
}

void draw() {
  move(0, 0);
  turn ? printw("Player 2's turn:\n") : printw("Player 1's turn:\n");
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height / 2; j++) {
      for (int l = 0; l < 3; l++) {
        for (int k = 0; k < width; k++) {
          printw(" ");
        }
        if (l != 2) {
          printw("│");
        }
      }
      printw("\n");
    }

    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < (width - 1) / 2; k++) {
        printw(" ");
      }
      printw("%c", board[i][j]);
      for (int k = 0; k < width / 2; k++) {
        printw(" ");
      }
      if (j != 2) {
        printw("│");
      }
    }

    for (int j = 0; j < height / 2; j++) {
      printw("\n");
      for (int l = 0; l < 3; l++) {
        for (int k = 0; k < width; k++) {
          printw(" ");
        }
        if (l != 2) {
          printw("│");
        }
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
  move(y * (height + 1) + height / 2 + 1, x * (width + 1) + width / 2);
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
        move(y * (height + 1) + height / 2 + 1, x * (width + 1) + width / 2);
        break;
      case 66:    // key down
        if (y < 2) {
          y++;
        }
        move(y * (height + 1) + height / 2 + 1, x * (width + 1) + width / 2);
        break;
      case 67:    // key right
        if (x < 2) {
          x++;
        }
        move(y * (height + 1) + height / 2 + 1, x * (width + 1) + width / 2);
        break;
      case 68:    // key left
        if (x > 0) {
          x--;
        }
        move(y * (height + 1) + height / 2 + 1, x * (width + 1) + width / 2);
        break;
      case '\n':  // return
        if (board[y][x] == ' ') ch = -2;
        turn ? board[y][x] = P2_ : board[y][x] = P1_;
        break;
      case '-':
        if (width > 3 && height > 1) {
          width -= 5;
          height -= 2;
          clear();
          draw();
        }
        break;
      case '+':
        width += 5;
        height += 2;
        clear();
        draw();
        break;
    }
  }
}

int win() {
  for (int i = 0; i < 3; i++) {
    if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
      return (board[i][0] == P1_) ? 1 : 2;
    }
  }
  
  for (int j = 0; j < 3; j++) {
    if (board[0][j] != ' ' && board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
      return (board[0][j] == P1_) ? 1 : 2;
    }
  }
  
  if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
    return (board[0][0] == P1_) ? 1 : 2;
  }
  
  if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
    return (board[0][2] == P1_) ? 1 : 2;
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

    select();
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
