# Find the Cairo library
#
# This defines:
#
#    CAIRO_FOUND
#    CAIRO_GL_FOUND
#    CAIRO_INCLUDE_DIRS
#    CAIRO_GL_INCLUDE_DIRS
#    CAIRO_LIBRARIES

find_library(CAIRO_LIBRARIES
  NAMES cairo
  PATHS
    $ENV{CAIRO_DIR}/lib
    $ENV{CAIRO_DIR}
    /usr/local/lib
    /usr/lib
    /opt/lib
  )
find_path(CAIRO_INCLUDE_DIRS
  NAMES cairo.h
  PATH_SUFFIXES cairo
  PATHS
    $ENV{CAIRO_DIR}/include
    $ENV{CAIRO_DIR}
    /usr/local/include
    /usr/include
    /opt/include
  )
find_path(CAIRO_GL_INCLUDE_DIRS
  NAMES cairo-gl.h
  PATH_SUFFIXES cairo
  PATHS
    $ENV{CAIRO_DIR}/include
    $ENV{CAIRO_DIR}
    /usr/local/include
    /usr/include
    /opt/include
  )

set (CAIRO_FOUND "NO")
if (CAIRO_LIBRARIES AND CAIRO_INCLUDE_DIRS)
  set (CAIRO_FOUND "YES")
else (CAIRO_LIBRARIES AND CAIRO_INCLUDE_DIRS)
  if (Cairo_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find Cairo")
  endif (Cairo_FIND_REQUIRED)
endif (CAIRO_LIBRARIES AND CAIRO_INCLUDE_DIRS)

set (CAIRO_GL_FOUND "NO")
if (CAIRO_LIBRARIES AND CAIRO_GL_INCLUDE_DIRS)
  set (CAIRO_GL_FOUND "YES")
endif (CAIRO_LIBRARIES AND CAIRO_GL_INCLUDE_DIRS)

# TODO: Check for EGL support
#  - locate symbols cairo_egl_device_create and cairo_gl_surface_create_for_egl