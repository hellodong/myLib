cmake_minimum_required(VERSION 3.5)

project(rbtree_test)

add_library(cMini SHARED rbtree.c)

add_executable(${PROJECT_NAME}  example.c)

target_link_libraries(${PROJECT_NAME} PRIVATE cMini)
