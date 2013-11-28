#include <err.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

struct graphics_state {
    Display   *dpy;
    Window     window;
};

static void
cleanup (void *data)
{
    struct graphics_state *state = (graphics_state*)data;

    /*
      XDestroyWindow (state->dpy, state->window);
      XCloseDisplay (state->dpy);
    */
    free (state);
}

cairo_surface_t *
create_cairo_surface_image (int width, int height)
{
    struct graphics_state *state;
    cairo_surface_t *cairo_surface;

    state = (graphics_state*) malloc (sizeof (struct graphics_state));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    /*
      state->dpy = XOpenDisplay (NULL);
      if (state->dpy == NULL) {
      warnx ("Failed to open display: %s\n", XDisplayName (0));
      return NULL;
      }
    */

    cairo_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                                width,
                                                height);

    if (cairo_surface_set_user_data (cairo_surface,
                                     (cairo_user_data_key_t *) cleanup,
                                     state,
                                     cleanup))
    {
        warnx ("Failed to setup cleanup callback closure\n");
        cleanup (state);
        return NULL;
    }

    return cairo_surface;
}
