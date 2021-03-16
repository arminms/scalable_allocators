include_directories(../include)

# use dynamic link library when possible
if( NOT MSVC AND NOT VCPKG_PATH )
  add_definitions(-DBOOST_TEST_DYN_LINK)
endif()

function(add_named_test TEST_NAME TEST_SOURCE)
  get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_TARGET} ${TEST_SOURCE})
  target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)
  target_link_libraries(${TEST_TARGET}
    Boost::system
    Boost::unit_test_framework
    TBB::tbbmalloc
    TBB::tbb
  )
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_TARGET})
endfunction()

add_named_test("rmsd" test_rmsd.cpp)
add_named_test("norm2" test_norm2.cpp)