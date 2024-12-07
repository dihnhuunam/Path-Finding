cmake_minimum_required(VERSION 3.16)

project(PathFinding LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/home/dihnhuunam/Qt5.13.2/5.13.2/gcc_64/lib/cmake/Qt5")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/forms)

find_package(OpenSSL REQUIRED)
find_package(Qt5 COMPONENTS Widgets Quick QuickWidgets Gui Location Positioning REQUIRED)

include_directories(include)

add_executable(PathFinding
    src/main.cpp
    src/mainwindow.cpp
    src/findingalgorithm.cpp
    include/mainwindow.h
    include/findingalgorithm.h
    forms/mainwindow.ui
    resources/Resources.qrc
)

target_link_libraries(PathFinding PRIVATE
    Qt5::Widgets
    Qt5::Gui
    Qt5::Quick
    Qt5::QuickWidgets
    Qt5::Location
    Qt5::Positioning
    OpenSSL::SSL
    OpenSSL::Crypto
)
