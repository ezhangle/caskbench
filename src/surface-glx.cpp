#include <err.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

struct graphics_state {
  Display   *dpy;
  Window     window;

  GLXContext glx_context;
};

static void
cleanup (void *data)
{
  struct graphics_state *state = (graphics_state*)data;

  glXDestroyContext (state->dpy, state->glx_context);

  XDestroyWindow (state->dpy, state->window);
  XCloseDisplay (state->dpy);

  free (state);
}

cairo_surface_t *
create_source_surface_glx (int width, int height)
{
  struct graphics_state *state;
  cairo_device_t *cairo_device;
  cairo_surface_t *cairo_surface;

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
  XSetWindowAttributes xattr;

  state = (graphics_state*) malloc (sizeof (struct graphics_state));
  if (!state) {
    warnx ("Out of memory\n");
    return NULL;
  }

  state->dpy = XOpenDisplay (NULL);
  if (state->dpy == NULL) {
    warnx ("Failed to open display: %s\n", XDisplayName (0));
    return NULL;
  }

  visinfo = glXChooseVisual (state->dpy, DefaultScreen (state->dpy), rgba_attribs);
  if (visinfo == NULL) {
    warnx ("Failed to create RGBA and double-buffered visuals\n");
    XCloseDisplay (state->dpy);
    return NULL;
  }

  xattr.colormap = XCreateColormap (state->dpy,
				    RootWindow (state->dpy, visinfo->screen),
				    visinfo->visual, AllocNone);
  xattr.border_pixel = 0;
  state->window = XCreateWindow (state->dpy,
				 DefaultRootWindow (state->dpy),
				 100, 50,
				 width, height, 0, visinfo->depth,
				 InputOutput, visinfo->visual,
				 CWBorderPixel | CWColormap,
				 &xattr);
  XMapWindow (state->dpy, state->window);

  state->glx_context = glXCreateContext (state->dpy, visinfo, NULL, True);
  XFree (visinfo);

  if (state->glx_context == NULL) {
    warnx ("Could not create glx context\n");
    XCloseDisplay (state->dpy);
    return NULL;
  }

  cairo_device = cairo_glx_device_create (state->dpy, state->glx_context);
  cairo_gl_device_set_thread_aware (cairo_device, 0);
  cairo_surface = cairo_gl_surface_create_for_window (cairo_device,
						      state->window,
						      width,
						      height);
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
