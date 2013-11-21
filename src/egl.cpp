#include <err.h>
#include <stdlib.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "egl.h"

bool
createEGLContextWithWindow(egl_state_t *state)
{
  EGLint attr[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_RED_SIZE, 1,
    EGL_GREEN_SIZE, 1,
    EGL_BLUE_SIZE, 1,
    EGL_ALPHA_SIZE, 1,
    EGL_STENCIL_SIZE, 1,
    EGL_SAMPLES, 4,
    EGL_SAMPLE_BUFFERS, 1,
    EGL_NONE
  };
  EGLint ctx_attr[] = {
    EGL_CONTEXT_CLIENT_VERSION, 3,
    EGL_NONE
  };
  EGLConfig config;
  EGLint num;

  state->egl_display = eglGetDisplay ((EGLNativeDisplayType) state->dpy);
  if (state->egl_display == EGL_NO_DISPLAY) {
    warnx ("Cannot get egl display\n");
    return false;
  }

  if (! eglChooseConfig (state->egl_display, attr, &config, 1, &num)) {
    warnx ("Cannot choose EGL configuration\n");
    return false;
  }

  state->egl_context = eglCreateContext (state->egl_display, config, NULL, ctx_attr);
  if (state->egl_context == EGL_NO_CONTEXT) {
    warnx ("Cannot create EGL context\n");
    return false;
  }

  state->egl_surface = eglCreateWindowSurface (state->egl_display, config, state->window, NULL);
  if (state->egl_surface == EGL_NO_SURFACE) {
    warnx ("Cannot create EGL window surface\n");
    return false;
  }
  return true;
}

bool
initializeEGL(egl_state_t *state)
{
  EGLint major, minor;
  if (!eglInitialize(state->egl_display, &major, &minor)) {
    warnx ("Cannot initialize EGL\n");
    return false;
  }

  if (!eglBindAPI(EGL_OPENGL_ES_API)) {
    warnx ("Cannot bind EGL to GLES API\n");
    return false;
  }

  return true;
}
