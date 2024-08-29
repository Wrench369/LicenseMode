
set(unittestSrc
    LicenseManager_test.cpp
    RSACrypto_test.cpp
)

###########################################
# 3rdparty
###########################################
#Unit test
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../3rdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
enable_testing()
include_directories(GoogleTest)

add_executable(test_all ${unittestSrc})
target_link_libraries(test_all gtest gtest_main gmock)

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(NAME test COMMAND hello_test)
