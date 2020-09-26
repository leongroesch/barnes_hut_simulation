cmake_minimum_required(VERSION 3.10)

set(project_name "barnes_hut")

project(${project_name})

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${project_name} src/main.cpp src/body.cpp src/game_engine.cpp src/barnes_hut.cpp src/json_parser.cpp src/fps.cpp)

target_link_libraries(${project_name} sfml-graphics sfml-window sfml-system -ljsoncpp)