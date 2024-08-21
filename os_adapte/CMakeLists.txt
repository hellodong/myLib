cmake_minimum_required(VERSION 3.8)

project(OSGen CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if (MSVC)
    message("Compiler msvc")
elseif(UNIX)
    if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -fsanitize=address")
    elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Os")
    else()
        message("build type: ${CMAKE_BUILD_TYPE}")
    endif()
endif()

include_directories("./")

if (MSVC)
    message("Compiler msvc")
    include_directories("win/")
    set(PLATFORMS_SRCS win/d_win_time.cpp win/d_win_thread.cpp win/d_win_sem.cpp)
elseif(UNIX) 
    include_directories("posix/")
    set(PLATFORMS_SRCS posix/d_posix_time.cpp posix/d_posix_thread.cpp posix/d_posix_sem.cpp)
endif()

set(EXAMPLE1_SRCS example/example.cpp)
set(EXAMPLE2_SRCS example/example2.cpp)

add_library(${PROJECT_NAME} STATIC ${PLATFORMS_SRCS})
add_library(${PROJECT_NAME} SHARED ${PLATFORMS_SRCS})

if(UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()

add_executable(exampl1 ${EXAMPLE1_SRCS} ${PLATFORMS_SRCS})
add_executable(exampl2 ${EXAMPLE2_SRCS} ${PLATFORMS_SRCS})

#target_link_libraries(${PROJECT_NAME} PRIVATE cMini)
