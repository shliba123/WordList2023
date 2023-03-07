cmake_minimum_required(VERSION 3.20)
project(WordList2023)

set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE DIR_SRC "src/*.cpp")
file(GLOB_RECURSE HDR_SRC "src/*.h")

add_executable(WordList2023
        ${DIR_SRC} ${HDR_SRC})
