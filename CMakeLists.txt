cmake_minimum_required(VERSION 3.21)
project(LedED)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files in the 'src' directory
add_executable(LedED 
src/main.cpp 
src/editor.cpp
src/gui.cpp
src/menu.cpp
src/export.cpp
)

# Include the 'vendor/raylib' directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib)

# Link against the Raylib library
target_link_libraries(LedED PRIVATE raylib)