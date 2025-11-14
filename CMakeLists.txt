cmake_minimum_required(VERSION 3.10.0)
project(Tetris VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Tetris 
  src/main.cpp
  src/shapes.cpp
)

include(FetchContent)
set(RAYLIB_VERSION 5.5)

FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz 
)
	
set(BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)

FetchContent_MakeAvailable(raylib)

target_link_libraries(${PROJECT_NAME} raylib)