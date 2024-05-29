cmake_minimum_required(VERSION 3.16.2)
project(ITKBenchmarks NONE)

option(BENCHMARK_USE_ITKWasm "Enable ITK-Wasm performance benchmarking interfaces" OFF)
set(_itkwasm_deps)

if(BENCHMARK_USE_ITKWasm)
  list(APPEND _itkwasm_deps WebAssemblyInterface)
  add_definitions(-DPERFORMANCE_BENCHMARKING_USE_ITKWASM)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})
include(ITKBenchmarksExternalData)

ExternalData_Expand_Arguments(ITKBenchmarksData
  BRAIN_IMAGE
  "DATA{Data/Input/brainweb165a10f17.mha}"
)
cmake_host_system_information(RESULT HOSTNAME QUERY HOSTNAME)
string(TOLOWER "${HOSTNAME}" HOSTNAME_LOWER)
set(TEST_OUTPUT_DIR "${PROJECT_BINARY_DIR}/Testing")
set(BENCHMARK_RESULTS_OUTPUT_DIR "${PROJECT_BINARY_DIR}/../BenchmarkResults/${HOSTNAME_LOWER}")
file(MAKE_DIRECTORY ${BENCHMARK_RESULTS_OUTPUT_DIR})

include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

option(BENCHMARK_ITK_CORE "Test the performance of ITK Core." ON)

if(BENCHMARK_ITK_CORE)
  add_subdirectory(Core)
endif()

# option(BENCHMARK_ITK_FILTERING "Test the performance of ITK Filters." ON)

# if(BENCHMARK_ITK_FILTERING)
# add_subdirectory(Filtering)
# endif()

# option(BENCHMARK_ITK_REGISTRATION "Test the performance of ITK Registration." ON)

# if(BENCHMARK_ITK_REGISTRATION)
# add_subdirectory(Registration)
# endif()

# option(BENCHMARK_ITK_SEGMENTATION "Test the performance of ITK Segmentation." ON)

# if(BENCHMARK_ITK_SEGMENTATION)
# add_subdirectory(Segmentation)
# endif()
ExternalData_Add_Target(ITKBenchmarksData)
