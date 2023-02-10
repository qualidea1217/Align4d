cmake_minimum_required(VERSION 3.24)
project(MSA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(MSA MSA.cpp Align.cpp PreProcess.cpp PreProcess.h MSA.h PostProcess.cpp PostProcess.h Align.h)
