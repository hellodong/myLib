cmake_minimum_required(VERSION 3.5)

project(minilog C)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if (MSVC)
    message("Compiler msvc")
else()

if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -fsanitize=address")
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Os")
else()
    message("build type: ${CMAKE_BUILD_TYPE}")
endif()

endif()


#add_library(cMini SHARED rbtree.c)
include_directories("./src")

add_executable(${PROJECT_NAME}  example/main.c src/log.c src/log_buf.c src/log_file.c)

target_link_libraries(${PROJECT_NAME} pthread)

#target_link_libraries(${PROJECT_NAME} PRIVATE cMini)
