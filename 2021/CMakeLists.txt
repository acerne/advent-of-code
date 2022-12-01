set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/solutions)

SET(YEAR 2021)

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

# days

SUBDIRLIST(DAYS ${CMAKE_CURRENT_SOURCE_DIR}/)

FOREACH(DAY ${DAYS})
    project(${YEAR}-${DAY})
    message("Setting up project " ${PROJECT_NAME} "...")

    set(SRCS
        ${DAY}/Day.cpp
    )

    add_library(${PROJECT_NAME} SHARED ${SRCS})

    target_link_libraries(${PROJECT_NAME} 
        PRIVATE
        AocFramework
    )

ENDFOREACH()
