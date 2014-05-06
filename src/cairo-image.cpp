/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
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

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
