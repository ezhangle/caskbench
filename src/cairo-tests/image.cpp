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

#include "caskbench.h"
#include "caskbench_context.h"
#include <cairo-shapes.h>

static cairo_surface_t *image;

int
ca_setup_image(caskbench_context_t *ctx)
{
    image = cairoCreateSampleImage (ctx);
    return 1;
}

void
ca_teardown_image(void)
{
    cairo_surface_destroy (image);
}

int
ca_test_image(caskbench_context_t* ctx)
{
    cairo_t *cr = ctx->cairo_cr;

    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    int iw = cairo_image_surface_get_width (image);
    int ih = cairo_image_surface_get_height (image);
    int pw = w - iw;
    int ph = h - ih;

    for (int i=0; i<ctx->size; i++) {
        double x = (double)rand()/RAND_MAX * pw;
        double y = (double)rand()/RAND_MAX * ph;
        cairo_set_source_surface (cr, image, x, y);
#if 1
        cairo_paint (cr);
#else
        // Fill Method
        cairo_rectangle(cr, x, y, iw, ih);
        cairo_fill(cr);
#endif

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
