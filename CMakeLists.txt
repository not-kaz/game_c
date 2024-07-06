cmake_minimum_required(VERSION 3.28)
project(game_c C)

# Build cgltf library
add_library(cgltf STATIC ${PROJECT_SOURCE_DIR}/lib/cgltf/cgltf.c)
target_include_directories(cgltf PUBLIC ${PROJECT_SOURCE_DIR}/lib/cgltf)
target_compile_features(cgltf PUBLIC c_std_99)
add_subdirectory(lib/cglm)

# Build glad library
add_library(glad STATIC ${PROJECT_SOURCE_DIR}/lib/glad/src/gl.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/lib/glad/include)
target_compile_features(glad PUBLIC c_std_99)

# SDL2 library
set(SDL2_DIR ${PROJECT_SOURCE_DIR}/lib/sdl2/cmake)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2main)

# SDL2_image library
set(SDL2_image_DIR ${PROJECT_SOURCE_DIR}/lib/sdl2_image/cmake)
find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)

# Main project
file(GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c")
add_executable(game_c ${SOURCES})
target_compile_options(game_c PRIVATE -Wall -Wextra -std=c99 -Wpedantic -Wdeclaration-after-statement)
target_link_libraries(game_c PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image cgltf cglm glad)
target_include_directories(game_c PRIVATE ${PROJECT_SOURCE_DIR}/include)
