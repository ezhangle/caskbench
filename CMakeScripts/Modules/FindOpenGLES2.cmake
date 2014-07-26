#-------------------------------------------------------------------
# This file was derived from the CMake build system for OGRE
# (http://www.ogre3d.org/) from the following link:
#
#   http://gamekit.googlecode.com/svn/trunk/CMake/Packages/FindOpenGLES3.cmake
#
#-------------------------------------------------------------------
# OGRE License:
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find OpenGLES and EGL
# Once done this will define
#
#  OPENGLES2_FOUND        - system has OpenGLES
#  OPENGLES2_INCLUDE_DIR  - the GL include directory
#  OPENGLES2_LIBRARIES    - Link these to use OpenGLES
#
#  EGL_FOUND        - system has EGL
#  EGL_INCLUDE_DIR  - the EGL include directory
#  EGL_LIBRARIES    - Link these to use EGL


FIND_PATH(OPENGLES2_INCLUDE_DIR GLES2/gl2.h
  /usr/openwin/share/include
  /opt/graphics/OpenGL/include /usr/X11R6/include
  /usr/include
  )

FIND_LIBRARY(OPENGLES2_gl_LIBRARY
  NAMES GLESv2
  PATHS /opt/graphics/OpenGL/lib
  /usr/openwin/lib
  /usr/shlib /usr/X11R6/lib
  /usr/lib
  )

FIND_PATH(EGL_INCLUDE_DIR EGL/egl.h
  /usr/openwin/share/include
  /opt/graphics/OpenGL/include /usr/X11R6/include
  /usr/include
  )

FIND_LIBRARY(EGL_egl_LIBRARY
  NAMES EGL
  PATHS /opt/graphics/OpenGL/lib
  /usr/openwin/lib
  /usr/shlib /usr/X11R6/lib
  /usr/lib
  )

# On Unix OpenGL most certainly always requires X11.
# Feel free to tighten up these conditions if you don't
# think this is always true.
# It's not true on OSX.

IF (OPENGLES2_gl_LIBRARY)
  IF(NOT X11_FOUND)
    INCLUDE(FindX11)
  ENDIF(NOT X11_FOUND)
  IF (X11_FOUND)
    SET (OPENGLES2_LIBRARIES ${X11_LIBRARIES})
  ENDIF (X11_FOUND)
ENDIF (OPENGLES2_gl_LIBRARY)

SET( OPENGLES2_FOUND "YES" )
IF(OPENGLES2_gl_LIBRARY AND EGL_egl_LIBRARY)

    SET( OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} ${OPENGLES2_LIBRARIES})
    SET( EGL_LIBRARIES ${EGL_egl_LIBRARY} ${EGL_LIBRARIES})
    SET( OPENGLES2_FOUND "YES" )

ENDIF(OPENGLES2_gl_LIBRARY AND EGL_egl_LIBRARY)

MARK_AS_ADVANCED(
  OPENGLES2_INCLUDE_DIR
  OPENGLES2_gl_LIBRARY
  EGL_INCLUDE_DIR
  EGL_egl_LIBRARY
)
