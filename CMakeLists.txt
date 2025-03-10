cmake_minimum_required(VERSION 3.16)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt" PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)

project(
        StrataBro
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

message(STATUS "Building StrataBro version ${PROJECT_VERSION}")

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
        COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

add_subdirectory(src)