cmake_minimum_required(VERSION 3.23)

project(generate)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(generate ${CMAKE_CURRENT_SOURCE_DIR}/generate.cpp)