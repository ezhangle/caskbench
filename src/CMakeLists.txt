file(GLOB caskbench_SOURCES
  caskbench.cpp

  cairo-image.cpp
  skia-image.cpp

  cairo-tests/*.cpp
  skia-tests/*.cpp
  tests.cpp
)

if (CAIRO_GL_FOUND)
  list(APPEND caskbench_SOURCES
    glx.cpp
    cairo-glx.cpp
    skia-glx.cpp
  )
endif (CAIRO_GL_FOUND)

if (EGL_FOUND)
  list(APPEND caskbench_SOURCES
    egl.cpp
    cairo-egl.cpp
    skia-egl.cpp
  )
endif (EGL_FOUND)

add_executable(caskbench ${caskbench_SOURCES})
target_link_libraries(caskbench ${CASKBENCH_LIBS})
