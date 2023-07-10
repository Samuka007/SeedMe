cmake_minimum_required(VERSION 3.0.0)
project(SeedMe VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread")

#aux_source_directory(src SRCS)
add_subdirectory(src/encrypt)
include_directories(src src/database src/server src/encrypt src/util src/include)

add_executable(SeedMe src/main.cpp)
add_executable(Testme src/test.cpp)

target_sources(SeedMe PUBLIC ${sources})
target_link_libraries(SeedMe PUBLIC md5)

target_sources(Testme PUBLIC ${sources})
target_link_libraries(Testme PUBLIC md5)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
