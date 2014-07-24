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

int
ca_setup_radial_gradient(caskbench_context_t *ctx)
{
    cairo_set_antialias (ctx->cairo_cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width (ctx->cairo_cr, 1);
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

    // TODO: Implement radial gradient

    shapes_t shape;
    shape_copy(&ctx->shape_defaults, &shape);
    for (int i=0; i<ctx->size; i++) {
        double x1 = (double)rand()/RAND_MAX * w;
        double x2 = (double)rand()/RAND_MAX * w;
        double y1 = (double)rand()/RAND_MAX * h;
        double y2 = (double)rand()/RAND_MAX * h;

        shape.x = MIN(x1, x2);
        shape.y = MIN(x1, x2);
        shape.width = abs(x2 - x1);
        shape.height = abs(y2 - y1);

        cairoRandomizeColor(ctx);
        cairoDrawRectangle(ctx, &shape);
        switch (ctx->shape_defaults.fill_type) {
            case CB_FILL_NONE:
                cairo_stroke(ctx->cairo_cr);
                break;
            case CB_FILL_SOLID:
                cairo_fill(ctx->cairo_cr);
                break;
            default:
                break;
        }
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
