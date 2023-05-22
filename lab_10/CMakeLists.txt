cmake_minimum_required(VERSION 3.21)
project(lab_10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.2.4/mingw_64/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(lab_10
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Alg.cpp
        Alg.h
        Errors.cpp
        Errors.h
        funcs.cpp funcs.h)

target_link_libraries(lab_10
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
