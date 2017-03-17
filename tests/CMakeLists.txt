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
)

add_executable(tests ${SRC})
target_link_libraries(tests gtest gtest_main pthread perg)
