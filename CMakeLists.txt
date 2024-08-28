cmake_minimum_required(VERSION 3.10)

project(demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 OpenSSL 库
find_package(OpenSSL REQUIRED)

# 添加 OpenSSL 头文件目录
include_directories(${OPENSSL_INCLUDE_DIR})

###########################################
# 3rdparty
###########################################
#Unit test
add_subdirectory(3rdparty/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#base64
include_directories(3rdparty/base64/include)

#nlohmannJson
include_directories(3rdparty/nlohmannJson/include)

include_directories(LicenseModule/Public)
aux_source_directory(LicenseModule/Private SRC_LICENSECODE)

###########################################
# Main
###########################################
add_executable(${PROJECT_NAME} main.cpp ${SRC_LICENSECODE})

# 链接库
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

# add_subdirectory(test)
add_subdirectory(RegistrationTool)