cmake_minimum_required(VERSION 3.21)
project(LedED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(LedED 
src/main.cpp 
src/editor.cpp
src/gui.cpp
src/menu.cpp
src/export.cpp
src/toolbox.cpp
src/openfiledialog_impl.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib)

target_include_directories(LedED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/raygui)

target_link_libraries(LedED PRIVATE raylib)


#if (MSVC)
#    # Set the desired runtime library
#    string(CONCAT CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#    string(CONCAT CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
#endif()
