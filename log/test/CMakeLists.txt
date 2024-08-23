

add_executable(
    dlog_test
    ./src/minilog_test.cpp
)

target_compile_features(dlog_test PUBLIC cxx_auto_type)

include_directories("../src")

target_link_libraries(dlog_test PUBLIC dlog gtest pthread)

#include(GoogleTest)
#gtest_discover_tests(minilog_test)
