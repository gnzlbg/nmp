################################################################################
# Unit Tests
################################################################################
enable_testing()
add_custom_target(tests
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.+"
  COMMENT "Build and run all the unit tests.")

function(nmp_test name no_proc)
  string(REPLACE "${PROJECT_SOURCE_DIR}/" ";" FLS "${CMAKE_CURRENT_SOURCE_DIR}")
  list (GET FLS 1 FOLDER)
  string(REGEX REPLACE "/" "." BASE_NAME "${FOLDER}")
  set(TEST_NAME ${BASE_NAME}.${name})
  add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${name}.cpp)
  target_link_libraries(${TEST_NAME} ${NMP_LIBS})
  set(test_parameters -np ${no_proc} "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}")
  add_test(${TEST_NAME} mpirun ${test_parameters})
  add_dependencies(tests ${TEST_NAME})
endfunction(nmp_test)

nmp_test(traits 1)
nmp_test(env 2)
nmp_test(comm 2)
nmp_test(group 4)
nmp_test(send_recv 2)
