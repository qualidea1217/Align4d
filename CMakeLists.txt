cmake_minimum_required(VERSION 3.24)
project(MSA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(MSA MSA.cpp Align.cpp ProcessText.cpp ProcessText.h MSA.h)
