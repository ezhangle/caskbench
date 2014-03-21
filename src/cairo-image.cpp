#include <err.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

#include "image.h"

static image_state_t *state;

cairo_surface_t *
create_cairo_surface_image (int width, int height)
{
    cairo_surface_t *cairo_surface;

    state = (image_state_t*) malloc (sizeof (image_state_t));
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

    return cairo_surface;
}

void
destroy_cairo_image(void)
{
    cleanup_state_image(state);
}

void
update_cairo_image(void)
{
}
