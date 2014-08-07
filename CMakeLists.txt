cmake_minimum_required(VERSION 2.8)

project(caskbench)
set(CASKBENCH_VERSION_MAJOR 0)
set(CASKBENCH_VERSION_MINOR 4)
set(CASKBENCH_VERSION_MICRO 0)
set(PROJECT_VERSION "${CASKBENCH_VERSION_MAJOR}.${CASKBENCH_VERSION_MINOR}.${CASKBENCH_VERSION_MICRO}")
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
# Command line options
# ----------------------------------------------------------------------
option(ENABLE_SKIA "Build in skia tests" ON)
option(ENABLE_CAIRO "Build in cairo tests" ON)
option(ENABLE_GLX "Build against GLX for OpenGL" OFF)
option(ENABLE_EGL "Build against EGL for OpenGL" ON)
option(ENABLE_CAIROGLES "Use the cairogles fork API additions" OFF)

# ----------------------------------------------------------------------
# Dependencies
# ----------------------------------------------------------------------

find_package(PkgConfig REQUIRED)

find_package(Popt REQUIRED)
list(APPEND CASKBENCH_INCS_SYS ${POPT_INCLUDE_DIR})
list(APPEND CASKBENCH_LIBS ${POPT_LIBRARIES})
add_definitions(${POPT_DEFINITIONS})

find_package(X11 REQUIRED)
list(APPEND CASKBENCH_LIBS ${X11_LIBRARIES})

set(USE_CAIRO 0)
if (ENABLE_CAIRO)
  find_package(Cairo REQUIRED)
  message("Using cairo libraries at ${CAIRO_LIBRARIES}")
  list(APPEND CASKBENCH_INCS_SYS ${CAIRO_INCLUDE_DIRS})
  list(APPEND CASKBENCH_LIBS ${CAIRO_LIBRARIES})
  check_include_files(cairo/cairo-gl.h HAVE_CAIRO_GL_H)
  set(USE_CAIRO 1)
endif (ENABLE_CAIRO)

set(USE_SKIA 0)
if (ENABLE_SKIA)
  find_package(Skia REQUIRED)
  message("Skia found: ${SKIA_FOUND}")
  message("Using skia includes at ${SKIA_INCLUDE_DIRS}")
  list(APPEND CASKBENCH_INCS ${SKIA_INCLUDE_DIRS}/)
  message("Using skia libraries at ${SKIA_LIBRARIES}")
  list(APPEND CASKBENCH_LIBS ${SKIA_LIBRARIES})
  set(USE_SKIA 1)
endif (ENABLE_SKIA)

set(USE_GLX 0)
if (ENABLE_GLX)
  if (NOT HAVE_CAIRO_GL_H)
    message( FATAL_ERROR "For GLX support, cairo must be compiled with GL enabled" )
  endif (NOT HAVE_CAIRO_GL_H)

  find_package(OpenGL)
  if (NOT OPENGL_FOUND)
    message( FATAL_ERROR "GLX requires OpenGL, but none found" )
  endif (NOT OPENGL_FOUND)

  message("Using OpenGL libraries for GLX")
  list(APPEND CASKBENCH_INCS_SYS ${OpenGL_INCLUDE_DIRS})
  list(APPEND CASKBENCH_LIBS ${OpenGL_LIBRARY_DIRS})
  list(APPEND CASKBENCH_LIBS GL)
  check_include_files(GL/glx.h HAVE_GLX_H)
  add_definitions(${OpenGL_DEFINITIONS})
  set(USE_GLX 1)
endif (ENABLE_GLX)

set(USE_EGL 0)
if (ENABLE_EGL)
  set(EGL_LIBRARIES "")

  if (NOT HAVE_CAIRO_GL_H)
    message( FATAL_ERROR "For EGL support, cairo must be compiled with GL enabled" )
  endif (NOT HAVE_CAIRO_GL_H)

  find_package(OpenGLES2)
  if (OPENGLES2_FOUND)
    message("Using OpenGL ES 2 libraries")
    list(APPEND CASKBENCH_INCS_SYS ${OPENGLES2_INCLUDE_DIRS})
    list(APPEND EGL_LIBRARIES ${OPENGLES2_LIBRARIES})
    check_include_files(GLES2/gl2.h HAVE_GLES2_H)
    set(USE_EGL 1)
  endif (OPENGLES2_FOUND)

  find_package(OpenGLES3)
  if (OPENGLES3_FOUND)
    message("Using OpenGL ES 3 libraries")
    list(APPEND CASKBENCH_INCS_SYS ${OPENGLES3_INCLUDE_DIRS})
    list(APPEND EGL_LIBRARIES ${OPENGLES3_LIBRARIES})
    check_include_files(GLES3/gl3.h HAVE_GLES3_H)
    set(USE_EGL 1)
  endif (OPENGLES3_FOUND)

  if (EGL_LIBRARIES)
    list(REMOVE_DUPLICATES EGL_LIBRARIES)
    list(APPEND CASKBENCH_LIBS ${EGL_LIBRARIES})
    list(APPEND CASKBENCH_LIBS EGL)
  endif (EGL_LIBRARIES)
endif (ENABLE_EGL)

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
INSTALL(PROGRAMS ${CMAKE_SOURCE_DIR}/utils/flashcanvas DESTINATION bin)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SCRIPTS}/cmake_uninstall.cmake") 

# ----------------------------------------------------------------------
# Information Summary
# ----------------------------------------------------------------------
# cmake info
message("------------------------------------------------------------------------")
message("CMAKE_BINARY_DIR:        ${CMAKE_BINARY_DIR}")
message("CMAKE_SYSTEM_NAME:       ${CMAKE_SYSTEM_NAME}")
message("CMAKE_SYSTEM_VERSION:    ${CMAKE_SYSTEM_VERSION}")
message("CMAKE_SYSTEM_PROCESSOR:  ${CMAKE_SYSTEM_PROCESSOR}")
message("CMAKE_C_COMPILER:        ${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER:      ${CMAKE_CXX_COMPILER}")
message("CMAKE_BUILD_TYPE:        ${CMAKE_BUILD_TYPE}")

# project info
message("PROJECT_VERSION:         ${PROJECT_VERSION}")
message("ARCHIVE_NAME:            ${ARCHIVE_NAME}")

# dependency info
message("SKIA:                    ${ENABLE_SKIA} -> ${USE_SKIA}")
message("CAIRO:                   ${ENABLE_CAIRO} -> ${USE_CAIRO}")
message("GLX:                     ${ENABLE_GLX} -> ${USE_GLX}")
message("EGL:                     ${ENABLE_EGL} -> ${USE_EGL}")
message("OPENGLES2:               ${OPENGLES2_FOUND}")
message("OPENGLES3:               ${OPENGLES3_FOUND}")
message("CAIROGLES:               ${ENABLE_CAIROGLES}")
message("CAIRO:                   ${CAIRO_INCLUDE_DIRS}")
message("SKIA:                    ${SKIA_INCLUDE_DIRS}")
message("X11:                     ${X11_LIBRARIES}")
message("------------------------------------------------------------------------")

message("LIBS:")
foreach (lib ${CASKBENCH_LIBS})
  message("                         ${lib}")
endforeach (lib)
message("------------------------------------------------------------------------")
