cmake_minimum_required(VERSION 3.16)
project(level-editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    main_window.cpp
    main_window.h
    grid_widget.cpp
    grid_widget.h
    tile_toolbar.cpp
    tile_toolbar.h
)

add_executable(level-editor ${PROJECT_SOURCES})
target_link_libraries(level-editor PRIVATE Qt6::Widgets)

# Copy data directory to build directory
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
