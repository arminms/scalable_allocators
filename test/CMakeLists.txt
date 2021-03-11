include_directories(../include)

# use dynamic link library of boost test framework
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(test_rmsd test_rmsd.cpp)
target_compile_features(test_rmsd PRIVATE cxx_std_17)
target_link_libraries(test_rmsd
  Boost::system
  Boost::unit_test_framework
  ${TBB_IMPORTED_TARGETS}
)
add_test(NAME rmsd COMMAND test_rmsd)