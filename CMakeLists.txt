cmake_minimum_required(VERSION 2.8)

project(caskbench)
set(PROJECT_VERSION "0.2")
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeScripts/Modules/")

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

find_package(OpenGLES3)
list(APPEND CASKBENCH_INCS_SYS ${OPENGLES3_INCLUDE_DIRS})
list(APPEND CASKBENCH_LIBS ${OPENGLES3_LIBRARIES})

find_package(Cairo)
list(APPEND CASKBENCH_INCS_SYS ${CAIRO_INCLUDE_DIRS})
list(APPEND CASKBENCH_LIBS ${CAIRO_LIBRARIES})

# skia
set(SKIA_ROOT "$ENV{HOME}/src/skia/trunk" CACHE STRING "Location of skia source tree")
message(STATUS "Using skia from ${SKIA_ROOT}")
set(SKIA_LIB_PATHS 
  ${SKIA_ROOT}/out/Release/
  /usr/lib/x86_64-linux-gnu)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${SKIA_LIB_PATHS})
link_directories(${SKIA_LIB_PATHS})
list(APPEND CASKBENCH_INCS ${SKIA_ROOT}/include/)
list(APPEND CASKBENCH_INCS ${SKIA_ROOT}/include/core/)
list(APPEND CASKBENCH_INCS ${SKIA_ROOT}/include/config/)
list(APPEND CASKBENCH_INCS ${SKIA_ROOT}/include/tools/flags/)
list(APPEND CASKBENCH_INCS ${SKIA_ROOT}/include/include/lazy/)

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

add_subdirectory(src)

unset(CASKBENCH_INCS)
unset(CASKBENCH_INCS_SYS)

add_custom_target(dist
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
      | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
