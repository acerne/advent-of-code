cmake_minimum_required(VERSION 3.16)  

project(advent2021)         

set(CMAKE_CXX_STANDARD 17)        
set(CMAKE_BUILD_TYPE Release)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# main

add_executable(${CMAKE_PROJECT_NAME} framework/main.cpp )
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_DL_LIBS})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE    
    framework/ 
    framework/common/ 
    template/)

# days

SUBDIRLIST(DAYS ${CMAKE_CURRENT_SOURCE_DIR}/days/)

FOREACH(DAY ${DAYS})
    message("Setting up day " ${DAY} "...")
    add_library(${DAY} SHARED framework/common/day.cpp)
    target_include_directories(${DAY} PRIVATE 
        framework/common/ 
        days/${DAY}/)
    
    # solutions

    SUBDIRLIST(SOLUTIONS ${CMAKE_CURRENT_SOURCE_DIR}/days/${DAY}/solutions/)
    FOREACH(SOLUTION ${SOLUTIONS})
        message("...found solution: " ${SOLUTION})
        set(SOLUTION_LIB ${DAY}${SOLUTION})
        add_library(${SOLUTION_LIB} SHARED days/${DAY}/solutions/${SOLUTION}/solution.cpp)
        target_include_directories(${SOLUTION_LIB} PRIVATE 
            framework/common/  
            days/${DAY}/
            days/${DAY}/solutions/${SOLUTION}/)
        set_target_properties(${SOLUTION_LIB} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${DAY}/)
    ENDFOREACH()
ENDFOREACH()
