/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>
#include <cairo.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"

static cairo_surface_t *image;
static cairo_surface_t *cached_image;

int
ca_setup_image_rotate(caskbench_context_t *ctx)
{
    image = cairoCreateSampleImage (ctx);
    cached_image = cairoCacheImageSurface (ctx, image);
    return 1;
}

void
ca_teardown_image_rotate(void)
{
    cairo_surface_destroy (image);
    cairo_surface_destroy (cached_image);
}

int
ca_test_image_rotate(caskbench_context_t* ctx)
{
    cairo_t *cr = ctx->cairo_cr;
    static int counter = 0;
    double radian = 0;
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    int iw = cairo_image_surface_get_width (image);
    int ih = cairo_image_surface_get_height (image);
    int pw = w - iw;
    int ph = h - ih;


    for (int i=0; i<ctx->size; i++) {
        double x = (double)rnd()/RAND_MAX * pw;
        double y = (double)rnd()/RAND_MAX * ph;

        cairo_new_path(cr);
        cairo_save(cr);
        cairo_translate(cr, w/2, h/2);

        radian = (1/57.29) * (counter/50);
        cairo_rotate(cr, radian);

        cairo_translate(cr, -iw/2, -ih/2);
        cairo_set_source_surface (cr, cached_image, 0, 0);
        cairo_paint (cr);
        counter++;

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
