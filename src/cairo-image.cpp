#include <config.h>

#include <err.h>
#include <stdlib.h>
#include <cairo.h>

#include "forward.h"
#include "image.h"
#include "device_config.h"

static image_state_t *state;

cairo_surface_t *
create_cairo_surface_image (const device_config_t& config)
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
                                                config.width,
                                                config.height);

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
