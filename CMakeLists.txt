cmake_minimum_required(VERSION 3.7)

include(cmake/BuildType.cmake)

option(USE_VCPKG "Use vcpkg C++ library manager if exists ?" ON)
if(${USE_VCPKG} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(UNIX)
    find_file(VCPKG_PATH_TXT vcpkg.path.txt
      PATHS $ENV{HOME}
      PATH_SUFFIXES .vcpkg
      )
    if(VCPKG_PATH_TXT)
      file(READ ${VCPKG_PATH_TXT} VCPKG_PATH)
      set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
      message(STATUS "Using vcpkg CMake toolchain file: " ${CMAKE_TOOLCHAIN_FILE})
    endif()
  endif(UNIX)
  if(NOT DEFINED VCPKG_PATH)
    find_path(VCPKG_PATH .vcpkg-root
      PATHS $ENV{HOME} / /opt /opt/local /usr /usr/local /usr/local/share
      PATH_SUFFIXES vcpkg src/vcpkg
      )
    if(VCPKG_PATH)
      set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
      message(STATUS "Using vcpkg CMake toolchain file: " ${CMAKE_TOOLCHAIN_FILE})
    endif()
  endif()
endif()

project(Scalable-Allocators)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

# if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qopenmp-simd -ipo -O3 -xSSE4.2")
# endif()

option(BUILD_MISSING_DEPENDENCIES "Clone/Build missing dependencies ?" ON)

# Visual Studio C++ specific
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244") # Warning C4244: 'initializing': conversion from 'double' to 'int', possible loss of data
endif(MSVC)

# check/add compiler OpenMP support flags
find_package(OpenMP)
if (OPENMP_FOUND OR OPENMP_CXX_FOUND)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# find/clone Intel Threading Building Blocks (TBB) and ParallelSTL
if(CMAKE_TOOLCHAIN_FILE)
  find_package(TBB CONFIG REQUIRED)
  # find_package(ParallelSTL CONFIG REQUIRED)
  message(STATUS "Found Intel TBB library")
else(CMAKE_TOOLCHAIN_FILE)
  find_package(PSTL COMPONENTS tbbmalloc)
  if(NOT PSTL_FOUND AND BUILD_MISSING_DEPENDENCIES)
      include(external/tbb.cmake)
      include(external/parallelstl.cmake)
  endif(NOT PSTL_FOUND AND BUILD_MISSING_DEPENDENCIES)
endif(CMAKE_TOOLCHAIN_FILE)

# find/clone Google Benchmark
if(CMAKE_TOOLCHAIN_FILE)
  find_package(benchmark CONFIG REQUIRED)
  message(STATUS "Found Google Benchmark library")
  set(BENCHMARK_TARGET benchmark::benchmark)
else(CMAKE_TOOLCHAIN_FILE)
  find_package(benchmark
      CONFIG
      HINTS /usr/local /opt/local /opt $ENV{HOME}
    )
  include(external/benchmark.cmake)
endif(CMAKE_TOOLCHAIN_FILE)

add_subdirectory(src)
