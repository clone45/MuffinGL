cmake_minimum_required(VERSION 3.15)
project(MuffinGL_Project)

# Find SDL2 packages using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# MuffinGL library
add_library(muffinGL STATIC
    src/graphics.cpp
    src/raster.cpp
    src/texture.cpp
    src/layer.cpp
    src/camera.cpp
)

# Set include directories for the library
target_include_directories(muffinGL
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
)

# Link SDL2 to the library
target_link_directories(muffinGL
    PRIVATE
        ${SDL2_LIBRARY_DIRS}
        ${SDL2_IMAGE_LIBRARY_DIRS}
)

target_link_libraries(muffinGL
    PUBLIC
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Example executable
add_executable(muffinGL_example src/main.cpp)
target_link_libraries(muffinGL_example PRIVATE muffinGL)