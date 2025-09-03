cmake_minimum_required(VERSION 4.1)
project(tictactoe)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -O3 -Wformat -Wformat=2 -Wconversion -Werror)
endif()

add_executable(tictactoe src/main.cc)