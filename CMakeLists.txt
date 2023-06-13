cmake_minimum_required(VERSION 3.0.0)
project(newtest VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread")

aux_source_directory(src SRCS)
include_directories(src)

add_executable(SeedMe ${SRCS})
file(GLOB sources *.cpp *.h *.hpp)
target_sources(SeedMe PUBLIC ${sources})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
