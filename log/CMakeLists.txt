cmake_minimum_required(VERSION 3.5)

project(dlog)

#include(FetchContent)
#FetchContent_Declare(
#      googletest
#      URL https://github.com/google/googletest/archive/refs/tags/release-1.12.0.tar.gz
#)
#FetchContent_MakeAvailable(googletest)

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

find_package(GTest)

enable_testing()

add_library(${PROJECT_NAME} STATIC src/log.c src/log_buf.c src/log_file.c)

include_directories("./src")

add_executable(example example/main.c src/log.c src/log_buf.c src/log_file.c)

target_link_libraries(example pthread)

#target_link_libraries(${PROJECT_NAME} PRIVATE cMini)

add_subdirectory(test)

install(TARGETS ${POJECTNAME} ARCHIVE DESTINATION lib/)

