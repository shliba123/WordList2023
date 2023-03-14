cmake_minimum_required(VERSION 3.24.2)
project(WordList2023)

set(CMAKE_CXX_STANDARD 14)
file(GLOB_RECURSE DIR_SRC "src/*.cpp")
file(GLOB_RECURSE HDR_SRC "src/*.h")

add_executable(WordList2023
        ${DIR_SRC} ${HDR_SRC} src/control/parse.cpp src/control/parse.h src/control/util.cpp src/control/util.h src/control/control.cpp src/control/control.h src/control/io.cpp src/control/io.h src/core/graph.cpp src/core/graph.h)

# TODO
# using openmp
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    target_link_libraries(WordList2023 PRIVATE OpenMP::OpenMP_CXX)
endif()