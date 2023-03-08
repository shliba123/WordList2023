cmake_minimum_required(VERSION 3.24.2)
project(WordList2023)

set(CMAKE_CXX_STANDARD 14)
file(GLOB_RECURSE DIR_SRC "src/*.cpp")
file(GLOB_RECURSE HDR_SRC "src/*.h")

add_executable(WordList2023
        ${DIR_SRC} ${HDR_SRC} src/control/parse.cpp src/control/parse.h src/control/util.cpp src/control/util.h)
