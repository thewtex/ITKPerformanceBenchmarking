project(ITKBenchmarkCore)

find_package(ITK REQUIRED
  COMPONENTS
  PerformanceBenchmarking
  ITKCommon
  ${_itkwasm_deps}
)
include(${ITK_USE_FILE})

add_executable(thread-overhead thread-overhead.cxx)
target_link_libraries(thread-overhead PUBLIC ${ITK_LIBRARIES})
add_test(
  NAME thread-overhead-benchmark
  COMMAND thread-overhead
  ${BENCHMARK_RESULTS_OUTPUT_DIR}/__DATESTAMP__thread-overhead.json
  --iterations 1000)
set_property(TEST thread-overhead-benchmark APPEND PROPERTY LABELS Core)

# # performance tests should not be run in parallel
set_tests_properties(thread-overhead-benchmark PROPERTIES RUN_SERIAL TRUE)
