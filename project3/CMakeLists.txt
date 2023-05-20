cmake_minimum_required(VERSION 3.5)

project(cg-vector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp

        mainwindow.cpp
        mainwindow.h

        drawingarea.cpp
        drawingarea.h

        circle.cpp
        circle.h

        line.h
        line.cpp

        shape.h

        polygon.h
        polygon.cpp

        rectangle.h
        rectangle.cpp
        )

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cg-vector
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
else ()
    add_executable(cg-vector
            ${PROJECT_SOURCES}
            )
endif ()

target_link_libraries(cg-vector PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(cg-vector PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        )

install(TARGETS cg-vector
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cg-vector)
endif ()