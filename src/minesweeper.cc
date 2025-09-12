#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <vector>
#include <random>
#include <algorithm>

struct cell {
  bool open;
  bool flag;
  bool mine;
};

std::vector<std::vector<cell>> board;

int gamewidth = 16;
int gameheight = 8;
int x = 0;
int y = 0;
int bombs = 8;
int remaining = 120;
int flags = 0;

void placement() {
  if (bombs <= 0) return;
  std::vector<std::pair<int, int>> positions;
  for (int i = 0; i < gameheight; i++) {
    for (int j = 0; j < gamewidth; j++) {
      positions.push_back({i, j});
    }
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(positions.begin(), positions.end(), gen);

  for (int i = 0; i < bombs; i++) {
    int row = positions[i].first;
    int col = positions[i].second;
    board[row][col].mine = true;
  }
}

void init() {
  board.resize(gameheight, std::vector<cell>(gamewidth));
  for (int i = 0; i < gameheight; i++) {
    for (int j = 0; j < gamewidth; j++) {
      board[i][j].open = false;
      board[i][j].flag = false;
      board[i][j].mine = false;
    }
  }

  move(0, 0);
  printw("Number of bombs: %d, number of flags placed: %d\n", bombs, flags);
  printw("╔");
  for (int i = 0; i < gamewidth; i++) {
    printw("═══");
    if (i < gamewidth - 1) {
      printw("╤");
    }
  }
  printw("╗\n");
  
  for (int row = 0; row < gameheight; row++) {
    printw("║");
    for (int col = 0; col < gamewidth; col++) {
      printw(" X ");
      if (col < gamewidth - 1) {
        printw("│");
      }
    }
    printw("║\n");

    if (row < gameheight - 1) {
      printw("╟");
      for (int col = 0; col < gamewidth; col++) {
        printw("───");
        if (col < gamewidth - 1) {
          printw("┼");
        }
      }
      printw("╢\n");
    }
  }
  
  printw("╚");
  for (int i = 0; i < gamewidth; i++) {
    printw("═══");
    if (i < gamewidth - 1) {
      printw("╧");
    }
  }
  printw("╝\n");
  refresh();
  placement();
  remaining = (gameheight * gamewidth) - bombs;
  move(y * 2 + 2, x * 4 + 2);
}

void leak() {
  for (int i = 0; i < gameheight; i++) {
    for (int j = 0; j < gamewidth; j++) {
      move(i * 2 + 2, j * 4 + 2);
      if (board[i][j].mine) {
        printw("!");
      } else {
        printw(" ");
      }
      refresh();
    }
  }
}

int surround(int cx, int cy) {
  int counter = 0;
  
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int newx = cx + i;
      int newy = cy + j;

      if (newx >= 0 && newx < gamewidth && newy >= 0 && newy < gameheight) {
        if (board[newy][newx].mine) {
          counter++;
        }
      }
    }
  }

  return counter;
}

void flood() {
  std::vector<std::pair<int, int>> queue;
  queue.push_back({y, x});

  while (!queue.empty()) {
    auto current = queue.back();
    queue.pop_back();
    int cy = current.first;
    int cx = current.second;

    int mine_count = surround(cx, cy);

    if (mine_count > 0 && board[cy][cx].open == false && board[cy][cx].flag == false) {
      board[cy][cx].open = true;
      move(cy * 2 + 2, cx * 4 + 2);
      printw("%d", mine_count);
      refresh();
      remaining--;
      continue;
    }

    if (board[cy][cx].open == false && board[cy][cx].flag == false) {
      board[cy][cx].open = true;
      remaining--;
      move(cy * 2 + 2, cx * 4 + 2);
      printw(" ");
      refresh();

      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          if (i == 0 && j == 0) continue;
          
          int newx = cx + j;
          int newy = cy + i;
          
          if (newx >= 0 && newx < gamewidth && newy >= 0 && newy < gameheight) {
            if (!board[newy][newx].open && !board[newy][newx].flag && !board[newy][newx].mine) {
              queue.push_back({newy, newx});
            }
          }
        }
      }
    }
  }
}

void selection() {
  int ch = -1;
  while (ch != -2) {
	  ch = getch();
    switch(ch) {
      case 65:    // key up
        if (y > 0) y--;
        move(y * 2 + 2, x * 4 + 2);
        break;
      case 66:    // key down
        if (y < gameheight - 1) y++;
        move(y * 2 + 2, x * 4 + 2);
        break;
      case 67:    // key right
        if (x < gamewidth - 1) x++;
        move(y * 2 + 2, x * 4 + 2);
        break;
      case 68:    // key left
        if (x > 0) x--;
        move(y * 2 + 2, x * 4 + 2);
        break;
      case '\n':  // return
        if (board[y][x].open == false && board[y][x].flag == false) {
          ch = -2;
          if (board[y][x].mine) {
            move(0, 0);
            printw("You Lose! Press any key to exit.                            ");
            leak();
            refresh();
            getch();
            endwin();
            exit(0);
          }
          int counter = surround(x, y);

          if (counter == 0) {
            flood();
          } else {
            board[y][x].open = true;
            move(y * 2 + 2, x * 4 + 2);
            printw("%d", counter);
            refresh();
            remaining--;
          }
        }
        break;
      case 'f':  // flag
        if (board[y][x].flag) {
          board[y][x].flag = false;
          flags--;
          if (board[y][x].open == false) {
            ch = -2;
            printw("X");
            refresh();
          }
        } else {
          board[y][x].flag = true;
          flags++;
          if (board[y][x].open == false) {
            ch = -2;
            printw("F");
            refresh();
          }
        }
        break;
      case 'q':  // exit
        endwin();
        exit(0);
        break;
    }
  }
}

void gameloop() {
  while (true) {
    selection();
    move(0, 0);
    printw("Number of bombs: %d, number of flags placed: %d", bombs, flags);
    move(y * 2 + 2, x * 4 + 2);

    if (remaining == 0) {
      move(0, 0);
      printw("You Win! Press any key to exit.                            ");
      leak();
      refresh();
      getch();
      endwin();
      exit(0);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc > 1) gameheight = std::atoi(argv[1]);
  if (argc > 2) gamewidth = std::atoi(argv[2]);
  if (argc > 3) bombs = std::atoi(argv[3]);
  setlocale(LC_ALL, ""); // Generate locales if not working
  initscr();
  curs_set(2);
  noecho();

  init();
  gameloop();

	endwin();
  return 0;
}
