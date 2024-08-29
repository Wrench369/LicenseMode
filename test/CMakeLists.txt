
set(unittestSrc
    # test/test_main.cpp
    hello_test.cpp
)

###########################################
# 3rdparty
###########################################
#Unit test
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../3rdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(hello_test ${unittestSrc})
target_link_libraries(hello_test gtest gtest_main)

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(NAME test COMMAND hello_test)
