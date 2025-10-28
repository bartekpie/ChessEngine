cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#SFML

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)
add_subdirectory(MyLib)
#GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(main main/main.cpp)
target_include_directories(main PUBLIC MyLib/include)
target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE SFML::Graphics MojaBib)

enable_testing()

add_executable(
  test_
  test/hello_test.cc
)
target_include_directories(test_ PUBLIC MyLib/include)
target_link_libraries(
  test_ PRIVATE
  GTest::gtest_main
  SFML::Graphics
  MojaBib
)

include(GoogleTest)
gtest_discover_tests(test_)
