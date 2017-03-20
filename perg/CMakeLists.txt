project(libperg CXX)

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
  stage.h
)

add_library(libperg ${SRC})
