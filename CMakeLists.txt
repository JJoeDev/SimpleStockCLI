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
    message("JsonCPP was not found. Fetching and building JsonCPP...")

    include(ExternalProject)
    ExternalProject_Add(
        jsoncpp_external
        PREFIX ${CMAKE_BINARY_DIR}/external
        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )

    set(JSONCPP_DIR ${CMAKE_BINARY_DIR}/external/jsoncpp_external)
    set(JSONCPP_INCLUDE_DIRS ${JSONCPP_DIR}/include)
    set(JSONCPP_LIBRARIES ${JSONCPP_DIR}/lib/libjsoncpp.a)

    include_directories(${JSONCPP_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${src})
    add_dependencies(${PROJECT_NAME} jsoncpp_external)
    target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})
endif()
