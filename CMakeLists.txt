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

add_executable(snake src/snake.c)
target_link_libraries(snake ${CURSES_LIBRARIES})

add_executable(tetris src/tetris.c)
target_link_libraries(tetris ${CURSES_LIBRARIES})