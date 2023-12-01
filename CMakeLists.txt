cmake_minimum_required(VERSION 3.26)
project(advent-of-code C)

set(CMAKE_C_STANDARD 17)

MACRO(SUBDIRLIST result current_directory)
    FILE(GLOB children RELATIVE ${current_directory} ${current_directory}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${current_directory}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(years ${CMAKE_CURRENT_SOURCE_DIR}/src)
FOREACH(year ${years})
    SUBDIRLIST(days ${CMAKE_CURRENT_SOURCE_DIR}/src/${year})
    FOREACH(day ${days})
        add_executable(${year}_${day} src/${year}/${day}/main.c)
        set_target_properties(${year}_${day}
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${year}/${day}
        )
    ENDFOREACH()
ENDFOREACH()
