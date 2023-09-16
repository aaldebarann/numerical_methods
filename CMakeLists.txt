cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME labwork1)
project(${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
set(MP2_LIBRARY "${PROJECT_NAME}")
set(MP2_TESTS   "test_${PROJECT_NAME}")
set(MP2_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${MP2_INCLUDE}" gtest)

# BUILD
add_subdirectory(source)
add_subdirectory(test)
add_subdirectory(3rdparty/gtest)

include_directories(3rdparty/gtest/googletest/include)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)