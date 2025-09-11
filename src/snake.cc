#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <queue>
#include <algorithm>
#include <random>

enum Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT
} direction;

std::pair<int, int> directions[4] = {
  {-1, 0},
  {0, 1},
  {1, 0},
  {0, -1}
};

int score = 0;
int gamewidth = 16;
int gameheight = 8;
int x = 0;
int y = 0;
bool hitwall = false;

std::queue<std::pair<int, int>> snake;
std::pair<int, int> food;
std::vector<std::pair<int, int>> available;

void placefood() {
  if (available.empty()) return;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::size_t> dis(0, available.size() - 1);
  std::pair<int, int> randlocation = available[dis(gen)];
  move(randlocation.first, randlocation.second);
  food = randlocation;
  printw("◆");
}

void init() {
  direction = UP;
  x = (gamewidth + 2) / 2;
  y = (gameheight + 2) / 2;
  for (int i = 2; i <= gameheight + 1; i++) {
    for (int j = 1; j <= gamewidth; j++) {
      if (!(i == y && j == x)) {
        available.push_back({i, j});
      }
    }
  }
  snake.push({y, x});
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
  move(y, x);
  printw("○");
  placefood();
}

void selection() {
  int ch = -1;
  while (ch != -2) {
	  ch = getch();
    switch(ch) {
      case 65:    // key up
        if (direction != DOWN) direction = UP;
        ch = -2;
        break;
      case 66:    // key down
        if (direction != UP) direction = DOWN;
        ch = -2;
        break;
      case 67:    // key right
        if (direction != LEFT) direction = RIGHT;
        ch = -2;
        break;
      case 68:    // key left
        if (direction != RIGHT) direction = LEFT;
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
  int oldx = x;
  int oldy = y;
  x += directions[direction].second;
  y += directions[direction].first;
  if (x < 0) hitwall = true;
  if (x >= gamewidth + 1) hitwall = true;
  if (y <= 1) hitwall = true;
  if (y > gameheight + 1) hitwall = true;
  snake.push({y, x});
  move(oldy, oldx);
  printw("●");
  auto it = std::find(available.begin(), available.end(), std::make_pair(y, x));
  if (it != available.end()) {
    available.erase(it);
  } else {
    hitwall = true;
  }
  if (y == food.first && x == food.second) {
    score++;
    placefood();
  } else {
    int tmpy = snake.front().first;
    int tmpx = snake.front().second;
    move(tmpy, tmpx);
    printw(" ");
    available.push_back({tmpy, tmpx});
    snake.pop();
  }
  move(y, x);
  printw("○");
  move(0, 0);
  printw("Score = %d\n", score);
  refresh();
}

bool win() {
  if (available.empty()) return true;
  return false;
}

void gameloop() {
  while (true) {
    bool result = win();
    if (hitwall || result) {
      move(0, 0);
      if (result) {
        printw("You Win! Your score was %d! Press any key to exit.", score);
      } else {
        printw("You Lose! Your score was %d! Press any key to exit.", score);
      }
      refresh();
      getch();
      break;
    }

    selection();
    moving();
  }
}

int main(int argc, char* argv[]) {
  if (argc > 1) gameheight = std::atoi(argv[1]);
  if (argc > 2) gamewidth = std::atoi(argv[2]);
  setlocale(LC_ALL, ""); // Generate locales if not working
  initscr();
  curs_set(0);
  noecho();

  init();
  gameloop();

	endwin();
  return 0;
}
