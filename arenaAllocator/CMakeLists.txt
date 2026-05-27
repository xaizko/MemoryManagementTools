cmake_minimum_required(VERSION 3.15)
project(ArenaAllocator LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -g)

add_executable(arena_test
	testArena.cpp
	Arena.cpp
)
