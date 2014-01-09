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

find_package(OpenGL)
if (OPENGL_FOUND)
  message("Using OpenGL libraries for GLX")
  list(APPEND CASKBENCH_INCS_SYS ${OpenGL_INCLUDE_DIRS})
  list(APPEND CASKBENCH_LIBS ${OpenGL_LIBRARY_DIRS})
  check_include_files(GL/glx.h HAVE_GLX_H)
  add_definitions(${OpenGL_DEFINITIONS})
endif (OPENGL_FOUND)

find_package(OpenGLES2)
if (OPENGLES2_FOUND)
  message("Using OpenGL ES 2 libraries")
  list(APPEND CASKBENCH_INCS_SYS ${OPENGLES2_INCLUDE_DIRS})
  list(APPEND CASKBENCH_LIBS ${OPENGLES2_LIBRARIES})
  check_include_files(GLES2/gl2.h HAVE_GLES2_H)
endif (OPENGLES2_FOUND)

find_package(OpenGLES3)
if (OPENGLES3_FOUND)
  message("Using OpenGL ES 3 libraries")
  list(APPEND CASKBENCH_INCS_SYS ${OPENGLES3_INCLUDE_DIRS})
  list(APPEND CASKBENCH_LIBS ${OPENGLES3_LIBRARIES})
  check_include_files(GLES3/gl3.h HAVE_GLES3_H)
endif (OPENGLES3_FOUND)

find_package(Cairo REQUIRED)
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
  skia
  freetype
  fontconfig
  pthread
)

if (OPENGL_FOUND)
  list(APPEND CASKBENCH_LIBS GL)
endif (OPENGL_FOUND)

if (OPENGLES3_FOUND)
  list(APPEND CASKBENCH_LIBS EGL)
endif (OPENGLES3_FOUND)

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
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/src/caskbench DESTINATION bin)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SCRIPTS}/cmake_uninstall.cmake") 
