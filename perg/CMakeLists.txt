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
  intrusive.h
  mask_filter.h
  buffer.h
  perg.h
)

add_library(perg ${SRC})
