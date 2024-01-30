cmake_minimum_required(VERSION 3.14)  
project(BinNum VERSION 1.0)  

find_package(Catch2 3 REQUIRED)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROFILE_DIR "${CMAKE_BINARY_DIR}/profiles")

set(FUZZ_RUNS 100000)
set(FUZZ_JOBS 15)
add_executable(tests ./test/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_executable(app ./test/test.cpp)
target_link_libraries(app PRIVATE Catch2::Catch2WithMain)
add_custom_target(run
    COMMAND sh -c "time ./app"
    COMMAND rm app
    DEPENDS app
    COMMENT "Running ..."
)
set(DEBUG_OPTIONS
  -Wsign-conversion
  -Wconversion 
  -Weffc++
  -Wextra
  -Wformat=2
  -Wlong-long
  -Wmissing-include-dirs
  -Wredundant-decls
  -Wstrict-overflow
  -Wundef
  -Wnull-dereference
  -Walloca
  -Wcast-qual
  -Werror=vla
  -pedantic-errors
  -O0
  -Wall
  -Wextra
  -std=c++20
  -ggdb
  -g3
  -DDEBUG
  -fsanitize=address
  -fno-omit-frame-pointer
)

# Use the common options for both compilation and linking
add_executable(app_debug ./test/test.cpp)
target_link_libraries(app_debug PRIVATE Catch2::Catch2WithMain)
target_compile_options(app_debug PRIVATE ${DEBUG_OPTIONS})
target_link_options(app_debug PRIVATE ${DEBUG_OPTIONS})
add_custom_target(debug
    COMMAND sh -c "time ./app_debug"
    COMMAND rm app_debug
    DEPENDS app_debug
    COMMENT "Running ..."
)
# NOTE: Profiling
add_executable(app_profile ./test/test.cpp)
target_link_libraries(app_profile PRIVATE Catch2::Catch2WithMain)
target_compile_options(app_profile PRIVATE -pg)
target_link_options(app_profile PRIVATE -pg)
string(TIMESTAMP PROFILE_TIMESTAMP "%Y-%m-%d_%H-%M-%S")
add_custom_target(profile
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROFILE_DIR}
    COMMAND app_profile
    COMMAND gprof app_profile gmon.out > ${PROFILE_DIR}/${PROFILE_TIMESTAMP}.txt
    COMMAND rm gmon.out app_profile
    DEPENDS app_profile
    COMMENT "Running profiling..."
)

# NOTE: Fuzzing BigInt(int) constructor and operations with address + ub sanitizer
add_executable(app_fuzz_int_addr_ub ./test/fuzz_int.cpp)
target_link_libraries(app_fuzz_int_addr_ub Catch2::Catch2WithMain)
target_compile_options(app_fuzz_int_addr_ub PRIVATE -g -fsanitize=fuzzer,address,undefined)
target_link_options(app_fuzz_int_addr_ub PRIVATE -g -fsanitize=fuzzer,address,undefined)
set_target_properties(app_fuzz_int_addr_ub PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/fuzz
)
add_custom_target(fuzz_int_addr_ub_finite
  COMMAND app_fuzz_int_addr_ub -jobs=${FUZZ_JOBS} -runs=${FUZZ_RUNS}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/fuzz/logs 
  COMMAND rm ${CMAKE_BINARY_DIR}/fuzz/app_fuzz_int_addr_ub
  DEPENDS app_fuzz_int_addr_ub
  COMMENT "Running fuzzing for BigInteger(int) constructor and basic operations. \n${FUZZ_RUNS} inputs..."
)
add_custom_target(fuzz_int_addr_ub_infinite
  COMMAND app_fuzz_int_addr_ub -jobs=${FUZZ_JOBS}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/fuzz/logs 
  COMMAND rm ${CMAKE_BINARY_DIR}/fuzz/app_fuzz_int_addr_ub
  DEPENDS app_fuzz_int_addr_ub
  COMMENT "Running fuzzing for BigInteger(int) constructor and basic operations. \nInfinite inputs..."
)

# NOTE: Fuzzing BigInt(str) constructor and operations with address + ub sanitizer
add_executable(app_fuzz_str_addr_ub ./test/fuzz_str.cpp)
target_compile_options(app_fuzz_str_addr_ub PRIVATE -g -fsanitize=fuzzer,address,undefined)
target_link_options(app_fuzz_str_addr_ub PRIVATE -g -fsanitize=fuzzer,address,undefined)
target_link_libraries(app_fuzz_str_addr_ub Catch2::Catch2WithMain)
set_target_properties(app_fuzz_str_addr_ub PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/fuzz
)
add_custom_target(fuzz_str_addr_ub_finite
  COMMAND app_fuzz_str_addr_ub -jobs=${FUZZ_JOBS} -runs=${FUZZ_RUNS} -use_value_profile=1 ${CMAKE_BINARY_DIR}/fuzz/corpus
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/fuzz/logs 
  COMMAND rm ${CMAKE_BINARY_DIR}/fuzz/app_fuzz_str_addr_ub
  DEPENDS app_fuzz_str_addr_ub
  COMMENT "Running fuzzing for BigInteger(string) constructor and basic operations. \n${FUZZ_RUNS} inputs..."
)
add_custom_target(fuzz_str_addr_ub_infinite
  COMMAND app_fuzz_str_addr_ub -jobs=${FUZZ_JOBS} -use_value_profile=1 ${CMAKE_BINARY_DIR}/fuzz/corpus
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/fuzz/logs 
  COMMAND rm ${CMAKE_BINARY_DIR}/fuzz/app_fuzz_str_addr_ub
  DEPENDS app_fuzz_str_addr_ub
  COMMENT "Running fuzzing for BigInteger(string) constructor and basic operations. \nInfinite inputs..."
)

# NOTE: Run fuzzing
add_custom_target(fuzz
    COMMAND echo "Running fuzz target: fuzz_str_addr_ub_finite"
    COMMAND ${CMAKE_COMMAND} --build . --target fuzz_str_addr_ub_finite
    COMMAND echo "Running fuzz target: fuzz_int_addr_ub_finite"
    COMMAND ${CMAKE_COMMAND} --build . --target fuzz_int_addr_ub_finite
    COMMENT "Running fuzz targets: fuzz_str_addr_ub_finite and fuzz_int_addr_ub_finite"
)
