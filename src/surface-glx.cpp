#include <err.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

struct graphics_state {
  Display *dpy;

  GLXContext glx_context;
};

static void
cleanup (void *data)
{
  struct graphics_state *state = (graphics_state*)data;

  glXDestroyContext (state->dpy, state->glx_context);
  XCloseDisplay (state->dpy);

  free (state);
}

cairo_surface_t *
create_source_surface_glx (int width, int height)
{
  int rgba_attribs[] = {
    GLX_RGBA,
    GLX_RED_SIZE, 1,
    GLX_GREEN_SIZE, 1,
    GLX_BLUE_SIZE, 1,
    GLX_ALPHA_SIZE, 1,
    GLX_DOUBLEBUFFER,
    None
  };
  XVisualInfo *visinfo;
  GLXContext ctx;
  struct graphics_state *state;
  cairo_device_t *device;
  cairo_surface_t *surface;
  Display *dpy;

  dpy = XOpenDisplay (NULL);
  if (dpy == NULL) {
    warnx ("Failed to open display: %s\n", XDisplayName (0));
    return NULL;
  }

  visinfo = glXChooseVisual (dpy, DefaultScreen (dpy), rgba_attribs);
  if (visinfo == NULL) {
    warnx ("Failed to choose glx visual\n");
    XCloseDisplay (dpy);
    return NULL;
  }

  ctx = glXCreateContext (dpy, visinfo, NULL, True);
  XFree (visinfo);

  if (ctx == NULL) {
    warnx ("Could not create glx context\n");
    XCloseDisplay (dpy);
    return NULL;
  }

  state = (graphics_state*) malloc (sizeof (struct graphics_state));
  if (!state) {
    warnx ("Out of memory\n");
    XCloseDisplay (dpy);
    return NULL;
  }
  state->dpy = dpy;
  state->glx_context = ctx;
  device = cairo_glx_device_create (dpy, ctx);
  if (cairo_device_set_user_data (device,
				  (cairo_user_data_key_t *) cleanup,
				  state,
				  cleanup))
    {
      warnx ("Failed to setup cleanup callback closure\n");
      cleanup (state);
      return NULL;
    }

  surface = cairo_gl_surface_create (device,
				     CAIRO_CONTENT_COLOR_ALPHA,
				     width, height);
  cairo_device_destroy (device);

  return surface;
}
