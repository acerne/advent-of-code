set(SRCS
    main.cpp
    Runner.cpp
)

add_executable(${PROJECT_NAME} ${SRCS}) 

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    AocFramework
)