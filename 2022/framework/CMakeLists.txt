cmake_minimum_required(VERSION 3.0.0)
project(AocFramework VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS
    factory/FactoryManager.cpp
    solution/Solution.cpp
)

add_library(${PROJECT_NAME} SHARED ${SRCS})

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/factory
    ${CMAKE_CURRENT_LIST_DIR}/solution
)