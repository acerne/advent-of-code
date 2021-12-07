cmake_minimum_required(VERSION 3.13)  
project(day03)               
set(CMAKE_CXX_STANDARD 20)            

set(SOURCE_FILES main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})