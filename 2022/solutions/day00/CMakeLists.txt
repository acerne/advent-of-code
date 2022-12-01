cmake_minimum_required(VERSION 3.16)  

project(Day00)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS
    ${PROJECT_NAME}.cpp
)

add_library(${PROJECT_NAME} SHARED ${SRCS})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    AocFramework
)

