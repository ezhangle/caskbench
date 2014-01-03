file(GLOB caskbench_SOURCES
  caskbench.cpp

  image.cpp
  cairo-image.cpp
  skia-image.cpp

  cairo-tests/*.cpp
  skia-tests/*.cpp
)

if (CAIRO_GL_FOUND)
  list(APPEND caskbench_SOURCES
    glx.cpp
    cairo-glx.cpp
    skia-glx.cpp
  )
endif (CAIRO_GL_FOUND)

if (OPENGLES3_FOUND)
  list(APPEND caskbench_SOURCES
    egl.cpp
    cairo-egl.cpp
    skia-egl.cpp
  )
endif (OPENGLES3_FOUND)

add_executable(generate_tests IMPORTED)
set_property(TARGET generate_tests PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/generate_tests")

set(GENERATED_TESTS ${CMAKE_CURRENT_BINARY_DIR}/tests.cpp)
add_custom_command(
  OUTPUT ${GENERATED_TESTS}
  COMMAND generate_tests ${CMAKE_CURRENT_SOURCE_DIR} ${GENERATED_TESTS}
  )
list(APPEND caskbench_SOURCES
  ${GENERATED_TESTS}
  )
add_executable(caskbench ${caskbench_SOURCES})
target_link_libraries(caskbench ${CASKBENCH_LIBS})
