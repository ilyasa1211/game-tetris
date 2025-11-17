cmake_minimum_required(VERSION 3.10.0)
project(Tetris VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Tetris 
  src/main.cpp
  src/shapes.cpp
)

include(FetchContent)
set(RAYLIB_VERSION 5.5)
set(GOOGLE_TEST_VERSION)

FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz 
)
	
set(BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)

FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz
)

FetchContent_MakeAvailable(googletest)

target_link_libraries(${PROJECT_NAME} raylib)

enable_testing()

add_executable(
  simple_test
  src/matrix_test.cpp
)
target_link_libraries(
  simple_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(simple_test)