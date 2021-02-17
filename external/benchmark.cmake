if(NOT benchmark_DIR)
    message(STATUS "Try cloning/building Google Benchmark...")
    execute_process(COMMAND git submodule update --init -- external/benchmark
      OUTPUT_QUIET
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL
      "Enable testing of the benchmark library."
    )
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/benchmark
      EXCLUDE_FROM_ALL
    )
    set(BENCHMARK_TARGET benchmark)
else()
    set(BENCHMARK_TARGET benchmark::benchmark)
endif()
