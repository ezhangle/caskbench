# Find the Cairo library
#
# This defines:
#
#    CAIRO_FOUND
#    CAIRO_INCLUDE_DIRS
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

set (CAIRO_FOUND "NO")
if (CAIRO_LIBRARIES AND CAIRO_INCLUDE_DIRS)
  set (CAIRO_FOUND "YES")
endif (CAIRO_LIBRARIES AND CAIRO_INCLUDE_DIRS)
