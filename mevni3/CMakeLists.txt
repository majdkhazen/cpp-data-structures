cmake_minimum_required(VERSION 3.19)

project(wet1)

set(CMAKE_CXX_STANDARD 11)

add_executable(Wet1 WorldCupTests.cpp catch.hpp Player.cpp Team.cpp AVLTree.h worldcup23a1.cpp wet1util.h)
