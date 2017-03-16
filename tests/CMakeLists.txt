project(tests CXX)

enable_testing()
include_directories (..)

set(SRC 
  queue_tests.cpp
  pipeline_tests.cpp
  glob_match_tests.cpp
)

add_executable(tests ${SRC})
target_link_libraries(tests gtest gtest_main pthread perg)
