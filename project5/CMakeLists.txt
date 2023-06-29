cmake_minimum_required(VERSION 3.25)
project(project5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(project5 main.cpp mainwindow.cpp drawingarea.cpp cylinder.cpp cylinder.h)
target_link_libraries(project5
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )