project(tests CXX)

enable_testing()
include_directories (..)

set(SRC 
  queue_tests.cpp
  pipeline_tests.cpp
  glob_match_tests.cpp
  mask_filter_tests.cpp
  view_tests.cpp
  intrusive_list_tests.cpp
  list_tests.cpp
  buffer_tests.cpp
  search_result_tests.cpp
  file_reader_tests.cpp
  rfile_reader_tests.cpp
  cyclic_list_tests.cpp
  queue_stress_tests.cpp
  raw_file_reader_tests.cpp
  forward_mask_filter_tests.cpp
  backward_mask_filter_tests.cpp
)

add_executable(tests ${SRC})
target_link_libraries(tests gtest gtest_main pthread libperg)
