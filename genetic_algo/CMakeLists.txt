cmake_minimum_required(VERSION 3.20)
project(genetic_algo_minimizer CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(SOURCES ${SOURCE_DIR}/main.cpp)
set(INCLUDES 
    ${INCLUDE_DIR}/gray_code.h
    ${INCLUDE_DIR}/genetic_optimizer.h
    ${INCLUDE_DIR}/genome.h
    ${INCLUDE_DIR}/offspring.h
)

add_library(genetic_minimizer ${INCLUDES})
target_include_directories(genetic_minimizer PUBLIC ${INCLUDE_DIR})
set_target_properties(genetic_minimizer PROPERTIES LINKER_LANGUAGE CXX)

add_executable(demoapp ${SOURCES})
target_link_libraries(demoapp PUBLIC genetic_minimizer ${Boost_LIBRARIES})
target_include_directories(demoapp PUBLIC ${Boost_INCLUDE_DIR})
set_target_properties(demoapp PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
