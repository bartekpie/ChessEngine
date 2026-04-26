add_library(MojaBib STATIC 
source/engine.cpp
source/generator.cpp
source/position.cpp)

target_include_directories(MojaBib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
 