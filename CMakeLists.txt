cmake_minimum_required(VERSION 3.16)

project(PathFinding LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/home/dihnhuunam/Qt5.13.2/5.13.2/gcc_64/lib/cmake/Qt5")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Quick QuickWidgets Gui Location Positioning REQUIRED)

add_executable(PathFinding
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Resources.qrc
)

target_link_libraries(PathFinding PRIVATE
    Qt5::Widgets
    Qt5::Gui
    Qt5::Quick
    Qt5::QuickWidgets
    Qt5::Location
    Qt5::Positioning
)

