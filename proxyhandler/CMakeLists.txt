cmake_minimum_required(VERSION 3.27)

project(proxyhandler VERSION 1.0  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)

set(SOURCES
    src/main.cpp
    src/curl_wrapper.cpp 
    src/curl_websockets_wrapper.cpp )

add_executable(proxyhandler ${SOURCES})

target_link_libraries(proxyhandler PRIVATE CURL::libcurl)

target_include_directories(proxyhandler PRIVATE ${CMAKE_SOURCE_DIR}/src)