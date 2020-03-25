cmake_minimum_required(VERSION 3.15)
project(DijkstraAlgorithm)

set(CMAKE_CXX_STANDARD 17)

add_executable(DijkstraAlgorithm main.cpp bheap.h graph_gen.h)