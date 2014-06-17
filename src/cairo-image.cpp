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

static image_state_t *state;
cairo_surface_t *cairo_surface;

static bool
convert_cairo_surface_to_ximage (cairo_surface_t *surface)
{
    unsigned char *data = cairo_image_surface_get_data (surface);
    state->image.width = cairo_image_surface_get_width (surface);
    state->image.height = cairo_image_surface_get_height (surface);
    state->image.format = ZPixmap;
    state->image.data = (char*)data;
    state->image.bitmap_unit = 4 * 8;
    state->image.byte_order = LSBFirst;
    state->image.bitmap_bit_order = LSBFirst;
    state->image.bitmap_pad = 4 * 8;
    state->image.depth = 32;
    state->image.bytes_per_line = state->image.width * 4;
    state->image.bits_per_pixel = 4 * 8;
    return XInitImage (&state->image);
}

cairo_surface_t *
create_cairo_surface_image (const device_config_t& config)
{
    state = (image_state_t*) malloc (sizeof (image_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!createImageWindow(state, config)) {
        cleanup_state_image(state);
        return NULL;
    }

    cairo_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                                config.width,
                                                config.height);

    return cairo_surface;
}

void
destroy_cairo_image(void)
{
    destroyImageWindow(state);
    cleanup_state_image(state);
}

void
update_cairo_image(void)
{
    if (convert_cairo_surface_to_ximage (cairo_surface)) {
        updateImageWindow(state);
    }
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
