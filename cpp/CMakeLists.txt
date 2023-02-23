cmake_minimum_required(VERSION 3.24)
project(MSA)

include_directories(C:/Users/peili/anaconda3/include)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "-O2")

add_executable(MSA msa.cpp align.cpp preprocess.cpp preprocess.h msa.h postprocess.cpp postprocess.h align.h)
