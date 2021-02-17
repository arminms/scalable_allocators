message(STATUS "Try cloning/building Intel ParallelSTL...")
execute_process(COMMAND git submodule update --init -- external/oneDPL
  OUTPUT_QUIET
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# disabling test targets
file(READ "${PROJECT_SOURCE_DIR}/external/oneDPL/CMakeLists.txt" CONTENT)
string(REPLACE "enable_testing()\nadd_subdirectory(test)"
  "#enable_testing()\n#add_subdirectory(test)"
  PATCHED "${CONTENT}"
)
file(WRITE "${PROJECT_SOURCE_DIR}/external/oneDPL/CMakeLists.txt"
  ${PATCHED}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/oneDPL
  EXCLUDE_FROM_ALL
)

# removing .git in external/oneDPL to not bugged by git about modified files
file(REMOVE_RECURSE "${PROJECT_SOURCE_DIR}/external/oneDPL/.git")
