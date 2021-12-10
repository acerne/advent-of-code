cmake_minimum_required(VERSION 3.13)  
project(advent2021)               
set(CMAKE_CXX_STANDARD 20)       

set(SOURCE_FILES 
    framework/main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} )
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_DL_LIBS})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE common/ framework/)

foreach(DAY day01 day02 day03 )
    add_library(${DAY} SHARED ${DAY}/solution.cpp)
    target_include_directories(${DAY} PRIVATE common/ ${DAY}/)
endforeach()