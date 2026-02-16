cmake_minimum_required(VERSION 3.28)
project(UnitTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
#GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

get_filename_component(MYLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}../../MyLib/include" ABSOLUTE)

enable_testing()

add_executable(
  Bitboard_
  bitboard.cc
  "${MyLIB_INCLUDE_DIR}bitboard.hpp"
 
)
target_include_directories(
  Bitboard_ PRIVATE
  "${MYLIB_INCLUDE_DIR}"
)
target_link_libraries(
  Bitboard_ PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Bitboard_)
