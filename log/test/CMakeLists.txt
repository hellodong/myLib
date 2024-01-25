
add_executable(
    minilog_test
    ./src/minilog_test.cpp
)

include_directories("../src")

target_link_libraries(
    minilog_test
    minilog
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(minilog_test)
