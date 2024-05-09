cmake_minimum_required(VERSION 3.20)
project(genetic_algo_minimizer)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(SOURCES ${SOURCE_DIR}/main.cpp)
set(INCLUDES ${INCLUDE_DIR}/gray_code.h)

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

add_subdirectory(test)
