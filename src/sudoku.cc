#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <vector>
#include <random>
#include <algorithm>

char board[9][9];
int x = 0;
int y = 0;
bool state = 1; // 0 = editing, 1 = moving
// 1-9 from lsb
uint16_t rows[9];
uint16_t cols[9];
uint16_t blocks[9];
int N = 42;

bool valid(int row, int col, char num) {
  int numint = num - '0';
  uint16_t bitmask = 1 << (numint - 1);
  int blockidx = (row / 3) * 3 + (col / 3);
  
  if ((rows[row] & bitmask) || (cols[col] & bitmask) || (blocks[blockidx] & bitmask)) {
    return false;
  }
  return true;
}

void fillBox(int row, int col) {
  std::vector<char> numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(numbers.begin(), numbers.end(), gen);

  int index = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      y = row + i;
      x = col + j;
      board[y][x] = numbers[index];
      int numint = numbers[index] - '0';
      uint16_t bitmask = 1 << (numint - 1);
      int blockidx = (y / 3) * 3 + (x / 3);
      rows[y] |= bitmask;
      cols[x] |= bitmask;
      blocks[blockidx] |= bitmask;
      move(y * 2 + 1, x * 4 + 2);
      printw("%c", board[y][x]);
      index++;
    }
  }
}

bool fillRest(int row, int col) {
  if (row == 9) return true;
  if (col == 9) return fillRest(row + 1, 0);
  if (board[row][col] != ' ') return fillRest(row, col + 1);

  for (char num = '1'; num <= '9'; num++) {
    if (valid(row, col, num)) {
      board[row][col] = num;
      move(row * 2 + 1, col * 4 + 2);
      printw("%c", board[row][col]);
      refresh();
      int numint = num - '0';
      uint16_t bitmask = 1 << (numint - 1);
      int blockidx = (row / 3) * 3 + (col / 3);
      rows[row] |= bitmask;
      cols[col] |= bitmask;
      blocks[blockidx] |= bitmask;

      if (fillRest(row, col + 1)) {
        return true;
      }

      board[row][col] = ' ';
      move(row * 2 + 1, col * 4 + 2);
      printw("%c", board[row][col]);
      refresh();
      rows[row] &= ~bitmask;
      cols[col] &= ~bitmask;
      blocks[blockidx] &= ~bitmask;
      refresh();
    }
  }

  return false;
}

void remove() {
  std::vector<std::pair<int, int>> positions;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      positions.push_back({i, j});
    }
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(positions.begin(), positions.end(), gen);

  for (int i = 0; i < N; i++) {
    int row = positions[i].first;
    int col = positions[i].second;
    int numint = board[row][col] - '0';
    board[row][col] = ' ';
    move(row * 2 + 1, col * 4 + 2);
    printw("%c", board[row][col]);
    refresh();
    uint16_t bitmask = 1 << (numint - 1);
    int blockidx = (row / 3) * 3 + (col / 3);
    rows[row] &= ~bitmask;
    cols[col] &= ~bitmask;
    blocks[blockidx] &= ~bitmask;
    refresh();
  }
}

void init() {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      board[i][j] = ' ';
    }
    rows[i] = 0;
    cols[i] = 0;
    blocks[i] = 0;
  }
  move(0, 0);
  printw("╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n");
  for (int i = 0; i < 9; i++) {
    printw("║");
    for (int j = 0; j < 9; j++) {
      printw(" %c ", board[i][j]);
      if (j == 2 || j == 5) {
        printw("║");
      } else if (j < 8) {
        printw("│");
      }
    }
    printw("║\n");
    
    if (i < 8) {
      if (i == 2 || i == 5) {
        printw("╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n");
      } else {
        printw("╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n");
      }
    }
  }
  printw("╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n");
  refresh();
  fillBox(0, 0);
  fillBox(3, 3);
  fillBox(6, 6);
  fillRest(0, 0);
  remove();
  move(y * 2 + 1, x * 4 + 2);
}

void edit(bool up) {
  if (up) {
    if (board[y][x] == ' ') {
      board[y][x] = '1';
    } else {
      board[y][x]++;
    }
    if (board[y][x] > '9') {
      board[y][x] = ' ';
    }
  } else {
    if (board[y][x] == ' ') {
      board[y][x] = '9';
    } else {
      board[y][x]--;
    }
    if (board[y][x] < '1') {
      board[y][x] = ' ';
    }
  }
  move(y * 2 + 1, x * 4 + 2);
  printw("%c", board[y][x]);
  refresh();
}

void game() {
  move(20, 0);
  for (int i = 0; i < 9; i++) {
    rows[i] = 0;
    cols[i] = 0;
    blocks[i] = 0;
  }
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (!valid(i, j, board[i][j]) || board[i][j] == ' ') {
        printw("You Lose! Press any key to exit.");
        refresh();
        getch();
        endwin();
        exit(0);
      }

      int numint = board[i][j] - '0';
      uint16_t bitmask = 1 << (numint - 1);
      int blockidx = (i / 3) * 3 + (j / 3);
      rows[i] |= bitmask;
      cols[j] |= bitmask;
      blocks[blockidx] |= bitmask;
    }
  }

  printw("You Win! Press any key to exit.");
  refresh();
  getch();
  endwin();
  exit(0);
}

void selection() {
  int ch = -1;
  while (ch != -2) {
	  ch = getch();
    switch(ch) {
      case 65:    // key up
        if (state) {
          if (y > 0) y--;
          move(y * 2 + 1, x * 4 + 2);
        } else {
          edit(true);
        }
        break;
      case 66:    // key down
        if (state) {
          if (y < 8) y++;
          move(y * 2 + 1, x * 4 + 2);
        } else {
          edit(false);
        }
        break;
      case 67:    // key right
        if (state) {
          if (x < 8) x++;
          move(y * 2 + 1, x * 4 + 2);
        } else {
          edit(true);
        }
        break;
      case 68:    // key left
        if (state) {
          if (x > 0) x--;
          move(y * 2 + 1, x * 4 + 2);
        } else {
          edit(false);
        }
        break;
      case '\n':  // return
        state = !state;
        ch = -2;
        break;
      case 's':  // submit
        game();
        ch = -2;
        break;
      case '?':  // help
        move(0, 0);
        printw("Controls: Arrow keys to move, Enter to place, s to submit, ? for help, q to quit\n");
        move(y * 2 + 1, x * 4 + 2);
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
    refresh();
    getch();
  }
}

int main(int argc, char* argv[]) {
  if (argc > 1) {
    int tmp = std::atoi(argv[1]);
    if (tmp > 0 && tmp < 82) N = tmp;
  }
  setlocale(LC_ALL, ""); // Generate locales if not working
  initscr();
  curs_set(2);
  noecho();

  init();
  gameloop();

	endwin();
  return 0;
}
