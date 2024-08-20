cmake_minimum_required(VERSION 3.8)

project(os_api CXX)

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
    set(SRCS example/example.cpp)
elseif(UNIX) 
    include_directories("posix/")
    set(PLATFORMS_SRCS posix/d_posix_time.cpp)
    set(SRCS example/example2.cpp)
    #target_link_libraries(${PROJECT_NAME} pthread)
endif()


add_executable(${PROJECT_NAME} ${SRCS} ${PLATFORMS_SRCS})

#target_link_libraries(${PROJECT_NAME} PRIVATE cMini)
