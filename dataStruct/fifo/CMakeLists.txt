cmake_minimum_required(VERSION 3.5)

project(fifo C CXX)

if (MSVC)
    message("Compiler msvc")
else()
    if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -fsanitize=address")
    elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Os")
    else()
        message("build type: ${CMAKE_BUILD_TYPE}")
    endif()
endif()

add_library(${PROJECT_NAME} STATIC fifo.c)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

include_directories("./")

add_executable(example example.c fifo.c)

add_subdirectory(legacy)
add_subdirectory(test)
