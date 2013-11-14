cmake_minimum_required(VERSION 2.8)

project(caskbench)
set(PROJECT_VERSION "0.2")
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CASKBENCH_LIBS "")
set(CASKBENCH_INCS "")
set(CASKBENCH_INCS_SYS "")

list(APPEND CASKBENCH_INCS
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/src

	# Generated includes
	${CMAKE_BINARY_DIR}/include
)

# ----------------------------------------------------------------------
# Dependencies
# ----------------------------------------------------------------------

find_package(PkgConfig REQUIRED)

find_package(Popt REQUIRED)
list(APPEND CASKBENCH_INCS_SYS ${POPT_INCLUDE_DIR})
list(APPEND CASKBENCH_LIBS ${POPT_LIBRARIES})
add_definitions(${POPT_DEFINITIONS})

find_package(X11 REQUIRED)
#list(APPEND CASKBENCH_LIBS ${X11_LIBRARIES})

find_package(Cairo)
list(APPEND CASKBENCH_INCS_SYS ${CAIRO_INCLUDE_DIRS})
list(APPEND CASKBENCH_LIBS ${CAIRO_LIBRARIES})

# skia
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}
  /home/bryce/src/skia/trunk/out/Release/
  /usr/lib/x86_64-linux-gnu)
link_directories(/home/bryce/src/skia/trunk/out/Release/
  /usr/lib/x86_64-linux-gnu)
list(APPEND CASKBENCH_INCS /home/bryce/src/skia/trunk/include/)
list(APPEND CASKBENCH_INCS /home/bryce/src/skia/trunk/include/core/)
list(APPEND CASKBENCH_INCS /home/bryce/src/skia/trunk/include/config/)
list(APPEND CASKBENCH_INCS /home/bryce/src/skia/trunk/include/tools/flags/)
list(APPEND CASKBENCH_INCS /home/bryce/src/skia/trunk/include/include/lazy/)

#OBJS="\
#    ${SKIA_ROOT}/out/Release/obj/tools/flags/flags.SkCommandLineFlags.o      \
#    ${SKIA_ROOT}/out/Release/obj.target/zlib/src/core/SkFlate.o              \
#    "
#

list(APPEND CASKBENCH_LIBS
  skia_core
  skia_pdf
  skia_opts
  skia_utils
  skia_views
  skia_xml
  skia_opts_ssse3
  skia_ports
  skia_sfnt
  skia_effects
  skia_images
  skia_skgpu
  skia_core
  freetype
  fontconfig
)

# ----------------------------------------------------------------------
# Build
# ----------------------------------------------------------------------

include_directories(${CASKBENCH_INCS})
include_directories(SYSTEM ${CASKBENCH_INCS_SYS})

#add_subdirectory(src)
#message("CMake Prefix: ${CMAKE_PREFIX_PATH}")
add_executable(caskbench
  src/caskbench.cpp

  src/cairo-tests/fill.cpp
  src/cairo-tests/image.cpp
  src/cairo-tests/mask.cpp
  src/cairo-tests/paint.cpp
  src/cairo-tests/stroke.cpp
  src/cairo-tests/rectangles.cpp
  src/cairo-tests/roundrect.cpp
  src/cairo-tests/bubbles.cpp
  src/cairo-tests/multishape.cpp

  src/skia-tests/fill.cpp
  src/skia-tests/image.cpp
  src/skia-tests/mask.cpp
  src/skia-tests/paint.cpp
  src/skia-tests/stroke.cpp
  src/skia-tests/rectangles.cpp
  src/skia-tests/roundrect.cpp
  src/skia-tests/bubbles.cpp
  src/skia-tests/multishape.cpp
  )
target_link_libraries(caskbench ${CASKBENCH_LIBS})

unset(CASKBENCH_INCS)
unset(CASKBENCH_INCS_SYS)

add_custom_target(dist
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
      | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
