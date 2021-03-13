cmake_minimum_required(VERSION 3.15)

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
      PATHS $ENV{USERPROFILE} /
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

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qopenmp-simd -ipo -O3 -axSSE4.2,SSE4.1,SSSE3,SSE3,SSE2")
endif()

# Visual Studio C++ specific
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244") # Warning C4244: 'initializing': conversion from 'double' to 'int', possible loss of data
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267") # Warning C4267: 'argument': conversion from 'size_t' to 'unsigned int', possible loss of data
endif(MSVC)

option(BUILD_MISSING_DEPENDENCIES "Clone/Build missing dependencies ?" ON)

# check/add compiler OpenMP support flags
find_package(OpenMP)
if (OPENMP_FOUND OR OPENMP_CXX_FOUND)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# find Intel Threading Building Blocks (TBB)
if(CMAKE_TOOLCHAIN_FILE)
  find_package(TBB CONFIG REQUIRED)
  message(STATUS "Found Intel TBB library")
else(CMAKE_TOOLCHAIN_FILE)
  find_package(TBB REQUIRED COMPONENTS tbbmalloc)
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
  if(NOT benchmark_DIR)
    if(BUILD_MISSING_DEPENDENCIES)
      include(external/benchmark.cmake)
    else(BUILD_MISSING_DEPENDENCIES)
      message(FATAL_ERROR "Could NOT find Google Benchmark library")
    endif(BUILD_MISSING_DEPENDENCIES)
  endif(NOT benchmark_DIR)
endif(CMAKE_TOOLCHAIN_FILE)

option(BUILD_TESTS "Build the unit tests ?" OFF)
if(${BUILD_TESTS})
  find_package(Boost 1.61 REQUIRED COMPONENTS system unit_test_framework)
  enable_testing()
  add_subdirectory(test)
endif()

add_subdirectory(src)
