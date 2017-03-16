project(perg CXX)

set(SRC 
  queue.h
  source.h
  sink.h
  proc.h
  channel.h
  pipe.h
  mask.h
  mask.cpp
)

add_library(perg ${SRC})
