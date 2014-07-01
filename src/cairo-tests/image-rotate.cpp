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

static cairo_surface_t *image;

int
ca_setup_image_rotate(caskbench_context_t *ctx)
{
    int i, x, y;
    cairo_t *cr_image;

    image = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 160, 40);
    cr_image = cairo_create (image);
    cairo_set_source_surface (cr_image, image, 0, 0);

    x = 5;
    for (i=0; i<16; i++) {
        x += 10;
        y = (i%2)*10;

        cairo_set_source_rgba (cr_image, 1.0/(i+1), 1.0, 16.0*i/255.0, 1.0);
        cairo_rectangle (cr_image, x, y, 10, 10);
        cairo_fill (cr_image);
    }

    cairo_destroy (cr_image);

    return 1;
}

void
ca_teardown_image_rotate(void)
{
    cairo_surface_destroy (image);
}

int
ca_test_image_rotate(caskbench_context_t* ctx)
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

        cairo_new_path(cr);
        cairo_save(cr);
        cairo_translate(cr, w/2, h/2);
        cairo_rotate(cr, i / 50.0);  // TODO: Original rotates by counter/50
        cairo_translate(cr, -iw/2, -ih/2);
        cairo_set_source_surface (cr, image, 0, 0);
#if 1
        cairo_paint (cr);
#else
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
