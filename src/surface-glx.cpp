#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

struct closure {
  Display *dpy;
  GLXContext ctx;
};

static void
cleanup (void *data)
{
  struct closure *arg = (closure*)data;

  glXDestroyContext (arg->dpy, arg->ctx);
  XCloseDisplay (arg->dpy);

  free (arg);
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
  struct closure *arg;
  cairo_device_t *device;
  cairo_surface_t *surface;
  Display *dpy;

  dpy = XOpenDisplay (NULL);
  if (dpy == NULL)
    return NULL;

  visinfo = glXChooseVisual (dpy, DefaultScreen (dpy), rgba_attribs);
  if (visinfo == NULL) {
    XCloseDisplay (dpy);
    return NULL;
  }

  ctx = glXCreateContext (dpy, visinfo, NULL, True);
  XFree (visinfo);

  if (ctx == NULL) {
    XCloseDisplay (dpy);
    return NULL;
  }

  arg = (closure*) malloc (sizeof (struct closure));
  if (!arg) {
    XCloseDisplay (dpy);
    return NULL;
  }
  arg->dpy = dpy;
  arg->ctx = ctx;
  device = cairo_glx_device_create (dpy, ctx);
  if (cairo_device_set_user_data (device,
				  (cairo_user_data_key_t *) cleanup,
				  arg,
				  cleanup))
    {
      cleanup (arg);
      return NULL;
    }

  surface = cairo_gl_surface_create (device,
				     CAIRO_CONTENT_COLOR_ALPHA,
				     width, height);
  cairo_device_destroy (device);

  return surface;
}
