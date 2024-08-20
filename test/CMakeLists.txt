
set(
    unittestSrc
    src/base/testing/test_main.cc
    src/base/hash/md5_unittest.cc
)

add_executable(base_unittest ${unittestSrc})
target_link_libraries(base_unittest gtest gtest_main)
target_link_libraries(base_unittest ${PROJECT_NAME})

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(NAME test COMMAND base_unittest)
