file(GLOB caskbench_SOURCES
  caskbench.cpp
  caskbench_context.cpp
  caskbench_result.cpp
  kinetics.cpp
  image.cpp
  shapes.cpp
  cairo-util.cpp
  cairo-image.cpp
  cairo-shapes.cpp
  cairo-tests/*.cpp
)

if (USE_GLX)
  list(APPEND caskbench_SOURCES
    glx.cpp
    cairo-glx.cpp
    )
endif (USE_GLX)

if (USE_EGL)
  list(APPEND caskbench_SOURCES
    egl.cpp
    cairo-egl.cpp
    )
endif (USE_EGL)

if (USE_SKIA)
  file(GLOB skia_files 
    skia-util.cpp
    skia-image.cpp
    skia-shapes.cpp
    skia-tests/*.cpp
    )
  list(APPEND caskbench_SOURCES ${skia_files} )
  if (USE_GLX)
    list(APPEND caskbench_SOURCES skia-glx.cpp)
  endif (USE_GLX)
  if (USE_EGL)
    list(APPEND caskbench_SOURCES skia-egl.cpp)
  endif (USE_EGL)
endif (USE_SKIA)

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
