cmake_minimum_required(VERSION 3.24)
project(gui)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/shliba/CLionProjects/WordList2023/gui")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(gui main.cpp)
target_link_libraries(gui
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

