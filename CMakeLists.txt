cmake_minimum_required(VERSION 3.27.0)
project(stockSim)

set(CMAKE_CXX_STANDARD 17)

set(src
    src/main.cpp
)

find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP jsoncpp)

if(JSONCPP_FOUND)
    include_directories(${JSONCPP_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${src})
    target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})
else()
    message(FATAL_ERROR "JsonCpp not found!")
endif()
