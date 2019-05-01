cmake_minimum_required(VERSION 3.14)
project(kaleidoscope)

set(CMAKE_CXX_STANDARD 14)

add_executable(kaleidoscope main.cpp lexer.cpp ast.cpp parser.cpp)