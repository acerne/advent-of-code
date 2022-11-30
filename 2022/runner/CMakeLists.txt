cmake_minimum_required(VERSION 3.16)  

project(AocRunner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS
    main.cpp
    Runner.cpp
)

add_executable(${PROJECT_NAME} ${SRCS}) 

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    AocFramework
)
