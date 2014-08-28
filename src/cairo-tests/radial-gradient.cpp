/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"
#include <math.h>

int
ca_setup_radial_gradient(caskbench_context_t *ctx)
{
    return 1;
}

void
ca_teardown_radial_gradient(void)
{
}

int
ca_test_radial_gradient(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;

    int stops = 2;
    cairo_pattern_t *pattern = cairo_pattern_create_radial (20,20,20,50,50,70);

    for (int i = 0; i < stops; i++) {
        double red, green, blue, alpha;
        generate_random_color(red,green,blue,alpha);
        cairo_pattern_add_color_stop_rgba (pattern, i/(stops-1), red, green, blue, alpha);
    }

    cairo_t *cr = ctx->cairo_cr;

    shapes_t shape;
    shape_copy(&ctx->shape_defaults, &shape);
    for (int i=0; i<ctx->size; i++) {
        double x1 = (double)rnd()/RAND_MAX * w;
        double x2 = (double)rnd()/RAND_MAX * w;
        double y1 = (double)rnd()/RAND_MAX * h;
        double y2 = (double)rnd()/RAND_MAX * h;
        double x = MIN(x1, x2);
        double y = MIN(y1, y2);

        double width_ratio = float(fabs(x2 - x1)) / float(100);
        double height_ratio = float(fabs(y2 - y1)) / float(100);

        cairo_save(cr);
        cairo_translate(cr,x, y);

        cairo_scale(cr, width_ratio, height_ratio);

        shape.x = 0;
        shape.y = 0;
        shape.width = 100;
        shape.height = 100;
        shape.fill_type = CB_FILL_LINEAR_GRADIENT;

        cairo_set_source (cr, pattern);
        cairoDrawRectangle(ctx, &shape);

        cairo_fill (cr);
        cairo_restore(cr);
    }
    cairo_pattern_destroy(pattern);

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
