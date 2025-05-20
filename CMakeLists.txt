cmake_minimum_required(VERSION 3.16)
project(level-editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

set(PROJECT_SOURCES
    main.cpp
    main_window.cpp
    main_window.h
    grid_widget.cpp
    grid_widget.h
    grid_selection.cpp
    grid_selection.h
    grid_undo.cpp
    grid_undo.h
    tile_toolbar.cpp
    tile_toolbar.h
    splash_screen.cpp
    splash_screen.h
    welcome_screen.cpp
    welcome_screen.h
)

add_executable(level-editor ${PROJECT_SOURCES})

target_link_libraries(level-editor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Copy data directory to build directory
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

# Copy Editor.md to build directory
file(COPY ${CMAKE_SOURCE_DIR}/Editor.md DESTINATION ${CMAKE_BINARY_DIR})
