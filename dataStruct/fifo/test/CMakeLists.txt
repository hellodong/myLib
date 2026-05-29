set(USER_HOME $ENV{HOME})
set(GTEST_ROOT "${USER_HOME}/local" CACHE PATH "GoogleTest installation directory")
list(APPEND CMAKE_PREFIX_PATH "${GTEST_ROOT}")

message(STATUS "GTEST_ROOT: ${GTEST_ROOT}")

# Try to find GTest, if not found, quit the build
find_package(GTest REQUIRED)

# Check if modern CMake targets are available, otherwise use legacy targets
if(TARGET GTest::gtest_main)
    set(GTEST_TARGETS GTest::gtest GTest::gtest_main)
else()
    set(GTEST_TARGETS GTest::GTest GTest::Main)
endif()

add_executable(
    fifo_test
    ./fifo_test.cpp
)

target_compile_features(fifo_test PUBLIC cxx_std_14)

include_directories("..")

target_link_libraries(fifo_test PUBLIC ${GTEST_TARGETS} pthread fifo)

include(GoogleTest)
gtest_discover_tests(fifo_test)
