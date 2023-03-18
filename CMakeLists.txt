cmake_minimum_required(VERSION 3.24.2)
project(WordList2023)

set(CMAKE_CXX_STANDARD 14)
file(GLOB_RECURSE DIR_SRC "src/*.cpp")
file(GLOB_RECURSE HDR_SRC "src/*.h")
# 设置代码覆盖率的相关参数
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fprofile-arcs -ftest-coverage -lgcov")

add_subdirectory(./lib)

include_directories(googletest/include
        googletest)


add_executable(WordList2023
        ${DIR_SRC} ${HDR_SRC} test/main_test.cpp)

target_link_libraries(WordList2023 gtest gtest_main)

add_library(WordList2023Shd SHARED src/core/core.cpp src/core/graph.cpp src/control/control.cpp
        src/control/io.cpp src/control/parse.cpp src/control/util.cpp)

#target_link_libraries(${PROJECT_NAME} profiler unwind)
#add_executable(gtest01 src/main_test.cpp)
#
#target_link_libraries(gtest01 gtest gtest_main)

