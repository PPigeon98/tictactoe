#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>

char board[3][3];
char smallboard[9][3][3];
const char P1_ = 'X';
const char P2_ = 'O';
bool turn = 0; // 0 = p1, 1 = p2
int bigx = 1;
int bigy = 1;
int x = 1;
int y = 1;
int width = 3;
int height = 1;
bool big = true;

void init() {
  for (int k = 0; k < 9; k++) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        smallboard[k][i][j] = ' ';
        board[i][j] = ' ';
      }
    }
  }
  move(0, 0);
  turn ? printw("Player 2's turn:\n") : printw("Player 1's turn:\n");
  
  printw("    │   │    ║    │   │    ║    │   │    \n");
  printw(" ───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n");
  printw("    │   │    ║    │   │    ║    │   │    \n");
  printw(" ───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n");
  printw("    │   │    ║    │   │    ║    │   │    \n");
  printw("═════════════╬═════════════╬═════════════\n");
  printw("    │   │    ║    │   │    ║    │   │    \n");
  printw(" ───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n");
  printw("    │   │    ║    │   │    ║    │   │    \n");
  printw(" ───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n");
  printw("    │   │    ║    │   │    ║    │   │    \n");
  printw("═════════════╬═════════════╬═════════════\n");
  printw("    │   │    ║    │   │    ║    │   │    \n");
  printw(" ───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n");
  printw("    │   │    ║    │   │    ║    │   │    \n");
  printw(" ───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n");
  printw("    │   │    ║    │   │    ║    │   │    \n");
  refresh();
    
  move(bigy * 6 + 3, bigx * 14 + 6);
}

void selection() {
  int ch = -1;
  while (ch != -2) {
	  ch = getch();
    switch(ch) {
      case 65:    // key up
        if (big) {
          if (bigy > 0) bigy--;
          move(bigy * 6 + 3, bigx * 14 + 6);
        } else {
          if (y > 0) y--;
          move(bigy * 6 + y * 2 + 1, bigx * 14 + x * 4 + 2);
        }
        break;
      case 66:    // key down
        if (big) {
          if (bigy < 2) bigy++;
          move(bigy * 6 + 3, bigx * 14 + 6);
        } else {
          if (y < 2) y++;
          move(bigy * 6 + y * 2 + 1, bigx * 14 + x * 4 + 2);
        }
        break;
      case 67:    // key right
        if (big) {
          if (bigx < 2) bigx++;
          move(bigy * 6 + 3, bigx * 14 + 6);
        } else {
          if (x < 2) x++;
          move(bigy * 6 + y * 2 + 1, bigx * 14 + x * 4 + 2);
        }
        break;
      case 68:    // key left
        if (big) {
          if (bigx > 0) bigx--;
          move(bigy * 6 + 3, bigx * 14 + 6);
        } else {
          if (x > 0) x--;
          move(bigy * 6 + y * 2 + 1, bigx * 14 + x * 4 + 2);
        }
        break;
      case '\n':  // return
        if (big) {
          big = false;
          x = 1;
          y = 1;
        } else {
          if (board[y][x] == ' ') ch = -2;
          turn ? (board[y][x] = P2_) : (board[y][x] = P1_);
          bigx = x;
          bigy = y;
        }
        break;
      case '?':  // help
        move(0, 0);
        printw("Controls: Arrow keys to move, Enter to place, i for instructions, ? for help, q to quit\n");
        move(bigy * 6 + 3, bigx * 14 + 6);
        break;
      case 'i':  // help
        move(0, 0);
        printw("Instructions: Step 1, pick a subgrid to play on. When placed in that subgrid, the next player has to play in the subgrid of the same position. When subgrid is finished(sombody won), and placement is in the subgrid, go back to previous subgrid. Otherwise, when unavailable(subgrid also won), player gets to pick the subgrid. Game wins when bigger grid is won\n");
        move(bigy * 6 + 3, bigx * 14 + 6);
        break;
      case 'q':  // exit
        endwin();
        exit(0);
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
