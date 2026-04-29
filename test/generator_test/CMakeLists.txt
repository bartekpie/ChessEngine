cmake_minimum_required(VERSION 3.28)
project(UnitTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "MYLIB_INCLUDE_DIR = ${MYLIB_INCLUDE_DIR}")
include(FetchContent)
#GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

get_filename_component(MYLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}../../../MyLib/include" ABSOLUTE)
get_filename_component(MYLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}../../../MyLib/source" ABSOLUTE)

enable_testing()

add_executable(
  Gen_
  gen.cc
  "${MYLIB_INCLUDE_DIR}/generator.hpp"
  "${MYLIB_INCLUDE_DIR}/position.hpp"
  "${MYLIB_INCLUDE_DIR}/bitboard.hpp"
  "${MYLIB_SOURCE_DIR}/generator.cpp"
)
target_include_directories(
  Gen_ PRIVATE
  "${MYLIB_INCLUDE_DIR}"
)
target_link_libraries(
  Gen_ PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Gen_)
