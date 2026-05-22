add_executable(
    fifo_test
    ./fifo_test.cpp
)

target_compile_features(fifo_test PUBLIC cxx_auto_type)

include_directories("..")

target_link_libraries(fifo_test PUBLIC gtest pthread)

#include(GoogleTest)
#gtest_discover_tests(fifo_test)
