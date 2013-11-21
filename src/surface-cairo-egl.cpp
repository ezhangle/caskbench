#include <err.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "egl.h"

static Display* getDisplay()
{
  static Display* display = NULL;
  if (!display)
    display = XOpenDisplay(NULL);
  return display;
}

static bool
createWindow(egl_state_t *state, int width, int height)
{
  state->dpy = XOpenDisplay (NULL);
  if (state->dpy == NULL) {
    warnx ("Failed to open display: %s\n", XDisplayName (0));
    return false;
  }

  state->window = XCreateSimpleWindow (state->dpy, DefaultRootWindow (state->dpy),
				       0, 0,
				       width, height,
				       0,
				       BlackPixel (state->dpy, DefaultRootWindow (state->dpy)),
				       BlackPixel (state->dpy, DefaultRootWindow (state->dpy)));
  XMapWindow (state->dpy, state->window);
  return true;
}

static void
cleanup (void *data)
{
  egl_state_t *state = (egl_state_t*)data;

  eglDestroyContext (state->egl_display, state->egl_context);
  eglDestroySurface (state->egl_display, state->egl_surface);
  eglTerminate (state->egl_display);

  XCloseDisplay (state->dpy);

  free (state);
}

cairo_surface_t *
create_source_surface_egl (int width, int height)
{
  egl_state_t *state;
  cairo_device_t *cairo_device;
  cairo_surface_t *cairo_surface;

  state = (egl_state_t*) malloc (sizeof (egl_state_t));
  if (!state) {
    warnx ("Out of memory\n");
    return NULL;
  }

  if (!createWindow(state, width, height)) {
    return NULL;
  }

  if (!initializeEGL(state)) {
    cleanup(state);
    return NULL;
  }

  if (!createEGLContextWithWindow(state)) {
    cleanup(state);
    return NULL;
  }

  cairo_device = cairo_egl_device_create (state->egl_display, state->egl_context);
  cairo_gl_device_set_thread_aware (cairo_device, 0);
  cairo_surface = cairo_gl_surface_create_for_egl (cairo_device,
						   state->egl_surface,
						   width, height);

  if (cairo_device_set_user_data (cairo_device,
                                  (cairo_user_data_key_t *) cleanup,
                                  state,
                                  cleanup))
    {
      warnx ("Failed to setup cleanup callback closure\n");
      cleanup (state);
      return NULL;
    }

  cairo_device_destroy (cairo_device);

  return cairo_surface;
}
