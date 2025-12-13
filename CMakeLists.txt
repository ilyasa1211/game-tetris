cmake_minimum_required(VERSION 3.10.0)
project(Tetris VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
endif()
file(GLOB SOURCES
    src/*.cpp
)

list(FILTER SOURCES EXCLUDE REGEX ".*test.cpp$")
add_executable(Tetris ${SOURCES})

include(FetchContent)
set(RAYLIB_VERSION 5.5)

FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz 
)
	
set(BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)

FetchContent_MakeAvailable(raylib)
target_link_libraries(${PROJECT_NAME} raylib)

if (NOT EMSCRIPTEN)
set(GOOGLE_TEST_VERSION)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz
)

FetchContent_MakeAvailable(googletest)
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
endif ()

