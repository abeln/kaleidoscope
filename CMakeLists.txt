cmake_minimum_required(VERSION 3.14)
project(kaleidoscope)
set(CMAKE_CXX_STANDARD 11)

set(ENV{LLVM_DIR} "/usr/local/opt/llvm")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(kaleidoscope main.cpp lexer.cpp ast.h parser.cpp lexer.h parser.h ast.cpp codegen.cpp codegen.h)

llvm_map_components_to_libnames(llvm_libs scalaropts support core irreader)

target_link_libraries(kaleidoscope ${llvm_libs})

