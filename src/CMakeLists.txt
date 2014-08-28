file(GLOB caskbench_SOURCES
  caskbench.cpp
  caskbench_context.cpp
  caskbench_result.cpp
  kinetics.cpp
  image.cpp
  shapes.cpp
  rnd.cpp
)

if (USE_GLX)
  list(APPEND caskbench_SOURCES
    glx.cpp
    )
endif (USE_GLX)

if (USE_EGL)
  list(APPEND caskbench_SOURCES
    egl.cpp
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

if (USE_CAIRO)
  file(GLOB cairo_files
    cairo-util.cpp
    cairo-image.cpp
    cairo-shapes.cpp
    cairo-tests/*.cpp
    )
  list(APPEND caskbench_SOURCES ${cairo_files} )
  if (USE_GLX)
    list(APPEND caskbench_SOURCES cairo-glx.cpp)
  endif (USE_GLX)
  if (USE_EGL)
    list(APPEND caskbench_SOURCES cairo-egl.cpp)
  endif (USE_EGL)
endif (USE_CAIRO)

add_executable(generate_tests IMPORTED)
set_property(TARGET generate_tests PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/generate_tests")

add_executable(generate_git_info IMPORTED)
set_property(TARGET generate_git_info PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/generate_git_info")

set(GENERATED_TESTS ${CMAKE_CURRENT_BINARY_DIR}/tests.cpp)
set(GIT_INFO ${CMAKE_CURRENT_BINARY_DIR}/git_info.cpp)

add_custom_command(
  OUTPUT ${GENERATED_TESTS}
  COMMAND generate_tests ${CMAKE_CURRENT_SOURCE_DIR} ${GENERATED_TESTS}
  )

add_custom_command(
  OUTPUT ${GIT_INFO}
  COMMAND generate_git_info ${CMAKE_CURRENT_SOURCE_DIR} ${GIT_INFO}
  )
list(APPEND caskbench_SOURCES
  ${GENERATED_TESTS}
  ${GIT_INFO}
  )
add_executable(caskbench ${caskbench_SOURCES})
target_link_libraries(caskbench ${CASKBENCH_LIBS})
