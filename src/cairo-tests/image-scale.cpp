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
#include "cairo-shapes.h"
#include <math.h>

static cairo_surface_t *image;
static cairo_surface_t *cached_image;

int
ca_setup_image_scale(caskbench_context_t *ctx)
{
    image = cairoCreateSampleImage (ctx);
    cached_image = cairoCacheImageSurface (ctx, image);
    return 1;
}

void
ca_teardown_image_scale(void)
{
    cairo_surface_destroy (image);
    cairo_surface_destroy (cached_image);
}

int
ca_test_image_scale(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;

    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    int iw = cairo_image_surface_get_width (image);
    int ih = cairo_image_surface_get_height (image);
    for (int i=0; i<ctx->size; i++) {
        double x1 = (double)rnd()/RAND_MAX * w;
        double x2 = (double)rnd()/RAND_MAX * w;
        double y1 = (double)rnd()/RAND_MAX * h;
        double y2 = (double)rnd()/RAND_MAX * h;
        double x = MIN(x1, x2);
        double y = MIN(y1, y2);
        double width_ratio = float(fabs(x2 - x1)) / float(iw);
        double height_ratio = float(fabs(y2 - y1)) / float(ih);

        cairo_save(cr);
        cairo_translate(cr,x,y);
        cairo_scale(cr, width_ratio, height_ratio);

        cairo_set_source_surface (cr, cached_image, 0, 0);
        // Scale without blur
        cairo_pattern_set_filter (cairo_get_source(cr), CAIRO_FILTER_FAST);
#if 1
        // Paint method
        cairo_paint (cr);
#else
        // Fill Method
        cairo_rectangle(cr, 0, 0, iw, ih);
        cairo_fill(cr);
#endif
        cairo_restore(cr);
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
