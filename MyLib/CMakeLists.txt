add_library(MojaBib STATIC 
source/bitboard.cpp
source/chesstate.cpp
source/console.cpp
source/engine.cpp
source/game.cpp
source/piece.cpp
source/visualization.cpp)
target_include_directories(MojaBib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(MojaBib PUBLIC SFML::Graphics)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}/bin/debug) 