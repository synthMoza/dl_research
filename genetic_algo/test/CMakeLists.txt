project(unit_tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

set(SOURCES gray_code.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} genetic_algo_minimizer GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
