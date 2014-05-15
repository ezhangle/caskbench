/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#if USE_EGL
# include <cairo-gl.h>
#endif

#include "caskbench.h"
#include "cairo-shapes.h"

static cairo_pattern_t *mask;

int
ca_setup_mask(caskbench_context_t *ctx)
{
    // See https://git.gnome.org/browse/gtk+/tree/gtk/gtkcolorbutton.c?id=804c80097995bb0026544118a32b59e23ee2fbe3#n241
    static unsigned char data[16] = { 0xFF, 0x00, 0x00, 0xFF,
                                      0x00, 0xFF, 0xFF, 0x00,
                                      0x00, 0xFF, 0xFF, 0x00,
                                      0xFF, 0x00, 0x00, 0xFF, };
    static cairo_surface_t *stippled = NULL;
    cairo_surface_t *cairo_surface;

    if (stippled == NULL) {
        stippled = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_A8, 2, 2, 4);
    }

#if USE_EGL
    cairo_device_t *cairo_device = cairo_surface_get_device (ctx->cairo_surface);
    if (cairo_surface_get_type (ctx->cairo_surface) == CAIRO_SURFACE_TYPE_IMAGE) {
        cairo_surface = cairo_image_surface_create (CAIRO_FORMAT_A8, 2, 2);
    } else if (cairo_surface_get_type (ctx->cairo_surface) == CAIRO_SURFACE_TYPE_GL) {
        /* create a gl surface the same size as image surface */
        cairo_surface = cairo_gl_surface_create (cairo_device, CAIRO_CONTENT_COLOR_ALPHA, 2, 2);
    }

    cairo_t *gl_ctx = cairo_create (cairo_surface);
    cairo_pattern_t *pattern = cairo_pattern_create_for_surface (stippled);
    cairo_set_source (gl_ctx, pattern);

    /* paint image surface to gl surface */
    cairo_paint (gl_ctx);
    cairo_destroy (gl_ctx);

    /* use gl surface as mask */
    mask = cairo_pattern_create_for_surface (cairo_surface);
#else
    mask = cairo_pattern_create_for_surface (stippled);
#endif
    cairo_pattern_set_extend (mask, CAIRO_EXTEND_REPEAT);
    cairo_pattern_set_filter (mask, CAIRO_FILTER_NEAREST);

    return 1;
}

void
ca_teardown_mask(void)
{
    cairo_pattern_destroy (mask);
}

int
ca_test_mask(caskbench_context_t *ctx)
{
    int i;
    cairo_t *cr = ctx->cairo_cr;

    for (i=0; i<ctx->size; i++) {
        // Apply mask on a circle
        cairoRandomizeColor(ctx);

        cairo_arc (cr, 40*i, 40, 30, 0, 2*M_PI);
        cairo_save (cr);
        cairo_clip (cr);
        cairo_mask (cr, mask);
        cairo_fill (cr);
        cairo_restore (cr);
    }
    return 1;
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
