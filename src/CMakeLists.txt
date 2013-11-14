file(GLOB caskbench_SOURCES
  caskbench.cpp

  cairo-tests/*.cpp
  skia-tests/*.cpp
)

add_executable(caskbench ${caskbench_SOURCES})
target_link_libraries(caskbench ${CASKBENCH_LIBS})
