cmake_minimum_required(VERSION 4.1)
project(snake_game)

set(CMAKE_CXX_STANDARD 20)

# Set the search path for Homebrew's SFML 2.6.2 package
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(snake_game main.cpp)

target_include_directories(snake_game PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(snake_game PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
