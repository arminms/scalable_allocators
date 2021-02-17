message(STATUS "Try cloning/building Intel TBB...")
execute_process(COMMAND git submodule update --init -- external/oneTBB
  OUTPUT_QUIET
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(TBB_ROOT ${PROJECT_SOURCE_DIR}/external/oneTBB)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/external/oneTBB/cmake)
include(${PROJECT_SOURCE_DIR}/external/oneTBB/cmake/TBBBuild.cmake)
tbb_build(TBB_ROOT ${TBB_ROOT} CONFIG_DIR TBB_DIR)
find_package(TBB REQUIRED tbb)

# removing .git in external/oneTBB to not bugged by git about modified files
file(REMOVE_RECURSE "${PROJECT_SOURCE_DIR}/external/oneTBB/.git")
