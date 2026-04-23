cmake_minimum_required(VERSION 3.29)
project(Lab_2_Sequence)

set(CMAKE_CXX_STANDARD 26)

add_executable(Lab_2_Sequence
        main.cpp
        types.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(SequenceTests
        tests/testMain.cpp
        tests/testArray.cpp
        tests/testList.cpp
        tests/testBit.cpp
)

target_link_libraries(SequenceTests
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(SequenceTests)