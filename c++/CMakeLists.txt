cmake_minimum_required(VERSION 3.13)  
project(advent2021)               
set(CMAKE_CXX_STANDARD 20)       

set(SOURCE_FILES 
    src/main.cpp
    src/day01.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/)