cmake_minimum_required(VERSION 3.15)
project(kalman1d_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)



# Src
add_executable(kalman1d_demo
    src/main.cpp
    src/data_generator.cpp
    src/kalman.cpp
    src/pipeline.cpp
    src/config.cpp
)

# GoogleTest
enable_testing()
add_executable(kalman1d_tests
    tests/kalman_test.cpp
    src/data_generator.cpp
    src/kalman.cpp
    src/pipeline.cpp
    src/config.cpp
)

# Benchmark
add_executable(kalman1d_benchmark
    src/pipeline_benchmark.cpp
    src/data_generator.cpp
    src/kalman.cpp
)

target_link_libraries(kalman1d_tests gtest gtest_main pthread)
add_test(NAME KalmanTests COMMAND kalman1d_tests)


