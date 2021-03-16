include_directories(../include)


set(BENCHMARKS
  generate_randoms
  rmsd
  norm2
)

foreach(BENCHMARK ${BENCHMARKS})
  set(PERF_TARGET perf_${BENCHMARK})
  add_executable(${PERF_TARGET} perf_${BENCHMARK}.cpp)
  target_compile_features(${PERF_TARGET} PRIVATE cxx_std_17)
  target_link_libraries(
    ${PERF_TARGET}
    ${BENCHMARK_TARGET}
    TBB::tbbmalloc
    TBB::tbb
  )
  install(TARGETS ${PERF_TARGET} RUNTIME DESTINATION bin COMPONENT ${PERF_TARGET})
endforeach()
