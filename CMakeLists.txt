cmake_minimum_required(VERSION 3.29)
project(XOXO)

set(CMAKE_CXX_STANDARD 20)

add_executable(XOXO main.cpp)
include_directories(/usr/local/include)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(XOXO sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
