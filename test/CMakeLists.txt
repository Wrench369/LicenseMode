file(GLOB TEST_SRCS "*_test.cpp")

# 查找 OpenSSL 库
find_package(OpenSSL REQUIRED)

###########################################
# 3rdparty
###########################################
#Unit test
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../3rdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
enable_testing()
include(CTest)
include_directories(GoogleTest)

foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC} ${unittestSrc})
    target_link_libraries(${TEST_NAME} gtest gtest_main gmock ${OPENSSL_LIBRARIES})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()