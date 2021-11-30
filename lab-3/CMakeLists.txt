cmake_minimum_required(VERSION 3.0)
project(Lab_3_1 C)

set(CMAKE_C_STANDARD 99)
set(PROJECT_HEADERS
        unit_test.h
        segment_table.h
        mmemory.h
        load_test.h
        adress_spaces.h
        )
set(PROJECT_SOURCES
        main.c
        adress_spaces.c
        load_test.c
        mmemory.c
        segment_table.c
        unit_test.c
        )

add_executable(Lab_3_1 ${PROJECT_SOURCES} ${PROJECT_HEADERS})
