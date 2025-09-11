cmake_minimum_required(VERSION 4.1)
project(tictactoe)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -O3 -Wformat -Wformat=2 -Wconversion -Werror)
endif()

set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)


add_executable(tictactoe src/tictactoe.c)
target_link_libraries(tictactoe ${CURSES_LIBRARIES})

add_executable(connect4 src/connect4.c)
target_link_libraries(connect4 ${CURSES_LIBRARIES})

add_executable(snake src/snake.cc)
target_link_libraries(snake ${CURSES_LIBRARIES})

add_executable(tetris src/tetris.c)
target_link_libraries(tetris ${CURSES_LIBRARIES})

add_executable(sudoku src/sudoku.cc)
target_link_libraries(sudoku ${CURSES_LIBRARIES})

add_executable(chess src/chess.c)
target_link_libraries(chess ${CURSES_LIBRARIES})

add_executable(bigtactoe src/bigtactoe.c)
target_link_libraries(bigtactoe ${CURSES_LIBRARIES})

add_executable(pong src/pong.c)
target_link_libraries(pong ${CURSES_LIBRARIES})

add_executable(minesweeper src/minesweeper.c)
target_link_libraries(minesweeper ${CURSES_LIBRARIES})

add_executable(stack src/stack.c)
target_link_libraries(stack ${CURSES_LIBRARIES})

add_executable(tron src/tron.c)
target_link_libraries(tron ${CURSES_LIBRARIES})