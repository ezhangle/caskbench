cmake_minimum_required(VERSION 2.8)

project(caskbench)
set(CASKBENCH_VERSION_MAJOR 0)
set(CASKBENCH_VERSION_MINOR 2)
set(PROJECT_VERSION "${CASKBENCH_VERSION_MAJOR}.${CASKBENCH_VERSION_MINOR}")
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
set(CMAKE_SCRIPTS "CMakeScripts")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/${CMAKE_SCRIPTS}/Modules/")

set(CASKBENCH_LIBS "")
set(CASKBENCH_INCS "")
set(CASKBENCH_INCS_SYS "")

include(CheckIncludeFiles)
list(APPEND CASKBENCH_INCS
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/src

	# Generated includes
	${CMAKE_BINARY_DIR}
	${CMAKE_BINARY_DIR}/src
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

check_include_files(GLES2/gl2.h HAVE_GLES2_H)

find_package(OpenGLES3)
list(APPEND CASKBENCH_INCS_SYS ${OPENGLES3_INCLUDE_DIRS})
list(APPEND CASKBENCH_LIBS ${OPENGLES3_LIBRARIES})
check_include_files(GLES3/gl3.h HAVE_GLES3_H)

find_package(Cairo)
list(APPEND CASKBENCH_INCS_SYS ${CAIRO_INCLUDE_DIRS})
list(APPEND CASKBENCH_LIBS ${CAIRO_LIBRARIES})
check_include_files(cairo/cairo-gl.h HAVE_CAIRO_GL_H)

# skia
# TODO: Replace with a find_package() call
#set(SKIA_ROOT "$ENV{HOME}/src/skia/trunk" CACHE STRING "Location of skia source tree")
set(SKIA_ROOT /usr)
set(SKIA_INCLUDE /usr/include/skia)
message(STATUS "Using skia from ${SKIA_ROOT}")
set(SKIA_LIB_PATHS 
  ${SKIA_ROOT}/out/Release/
  /usr/lib/
  /usr/lib/x86_64-linux-gnu)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${SKIA_LIB_PATHS})
link_directories(${SKIA_LIB_PATHS})
list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE}/)
list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE}/core/)
list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE}/config/)
list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE}/utils/)
list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE}/tools/flags/)
list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE}/lazy/)
list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE}/gpu/)
list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE}/gpu/gl/)

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
  pthread
  EGL
  GL
)

# ----------------------------------------------------------------------
# Build
# ----------------------------------------------------------------------

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CASKBENCH_INCS})
include_directories(SYSTEM ${CASKBENCH_INCS_SYS})

add_subdirectory(src)

unset(CASKBENCH_INCS)
unset(CASKBENCH_INCS_SYS)

# ----------------------------------------------------------------------
# Distribution
# ----------------------------------------------------------------------
add_custom_target(dist
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
      | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# ----------------------------------------------------------------------
# Installation / Uninstallation
# ----------------------------------------------------------------------
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_SCRIPTS}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SCRIPTS}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SCRIPTS}/cmake_uninstall.cmake") 
