file(GLOB caskbench_SOURCES
  caskbench.cpp

  glx.cpp
  egl.cpp
  cairo-*.cpp
  skia-*.cpp

  cairo-tests/*.cpp
  skia-tests/*.cpp
)

add_executable(caskbench ${caskbench_SOURCES})
target_link_libraries(caskbench ${CASKBENCH_LIBS})
