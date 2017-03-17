project(perg CXX)

include_directories (..)
set(SRC 
  main.cpp
)

add_executable(perg ${SRC})
target_link_libraries(perg libperg pthread)

