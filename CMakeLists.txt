cmake_minimum_required(VERSION 3.10)

project(demo)

include_directories(LicenseMode/Public)
aux_source_directory(LicenseMode/Private SRC_LICENSECODE)

###########################################
# 3rdparty
###########################################
#Unit test
add_subdirectory(3rdparty/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME} main.cpp ${SRC_LICENSECODE})

add_subdirectory(test)