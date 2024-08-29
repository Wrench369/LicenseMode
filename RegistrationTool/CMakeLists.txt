cmake_minimum_required(VERSION 3.10)

project(registrationTool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 OpenSSL 库
find_package(OpenSSL REQUIRED)

add_executable(register main_register.cpp)
add_executable(user main_demo.cpp)

# 链接库
target_link_libraries(register ${OPENSSL_LIBRARIES})
target_link_libraries(user ${OPENSSL_LIBRARIES})
