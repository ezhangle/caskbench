#include <err.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

struct graphics_state {
  Display   *dpy;
  Window     window;

  EGLDisplay egl_display;
  EGLContext egl_context;
  EGLSurface egl_surface;
};

static Display* getDisplay()
{
  static Display* display = NULL;
  if (!display)
    display = XOpenDisplay(NULL);
  return display;
}

static bool
createWindow(graphics_state *state, int width, int height)
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
  struct graphics_state *state = (graphics_state*)data;

  eglDestroyContext (state->egl_display, state->egl_context);
  eglDestroySurface (state->egl_display, state->egl_surface);
  eglTerminate (state->egl_display);

  XCloseDisplay (state->dpy);

  free (state);
}

static bool
createEGLContextWithWindow(struct graphics_state *state)
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

static bool
initializeEGL(struct graphics_state *state)
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

cairo_surface_t *
create_source_surface_egl (int width, int height)
{
  struct graphics_state *state;
  cairo_device_t *cairo_device;
  cairo_surface_t *cairo_surface;

  state = (graphics_state*) malloc (sizeof (struct graphics_state));
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
