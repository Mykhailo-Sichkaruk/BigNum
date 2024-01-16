cmake_minimum_required(VERSION 3.14)  
project(BinNum VERSION 1.0)  

find_package(Catch2 3 REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(tests ./test/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# NOTE: Profiling
add_executable(run ./test/test.cpp)
target_link_libraries(run PRIVATE Catch2::Catch2WithMain)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

set(BENCHMARK_OUTPUT_DIR "${CMAKE_BINARY_DIR}/profiles")
string(TIMESTAMP BENCHMARK_TIMESTAMP "%Y-%m-%d_%H-%M-%S")
add_custom_target(run_benchmark
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BENCHMARK_OUTPUT_DIR}
    COMMAND run
	COMMAND gprof run gmon.out > ${BENCHMARK_OUTPUT_DIR}/gprof_${BENCHMARK_TIMESTAMP}.txt
	COMMAND rm gmon.out run
    DEPENDS run
    COMMENT "Running benchmark..."
)
