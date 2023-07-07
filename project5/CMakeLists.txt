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

add_executable(project5
        main.cpp
        mainwindow.cpp
        mainwindow.h
        drawingarea.cpp
        drawingarea.h
        Math/Vector2.cpp
        Math/Vector2.h
        Math/Vector3.cpp
        Math/Vector3.h
        Math/Vector4.cpp
        Math/Vector4.h
        Math/Matrix.cpp
        Math/Matrix.h
        Transformations/Transformation.h
        Transformations/RotationMatrix.h
        Transformations/TranslationMatrix.h
        Models3D/Mesh.h
        Models3D/Cylinder.cpp
        Models3D/Cylinder.h
        Transformations/ScaleMatrix.h
        Models3D/Camera.h
        Models3D/Camera.cpp
        Models3D/Mesh.cpp Math/Vector.cpp Math/Vector.h)

target_link_libraries(project5
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )