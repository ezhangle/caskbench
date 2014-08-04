/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdlib.h>
#include <cairo.h>
#include <math.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"

static double delta = 0;

int
ca_setup_curves(caskbench_context_t *ctx)
{
    cairo_set_antialias (ctx->cairo_cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width (ctx->cairo_cr, 1);

    return 1;
}

void
ca_teardown_curves(void)
{
}

int
ca_test_curves(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;
    int cp = ctx->canvas_width / 2; // center point
    int rr = ctx->canvas_width / 2;
    int nn = 32; // must be even
    double step = (2 * M_PI) / nn;

    for (int i=0; i<ctx->size; i++) {
        double angle = delta;  // in radians
        double x = cp + rr * cos(angle);
        double y = cp + rr * sin(angle);

        // begin path
        cairo_new_path(cr);
        cairo_move_to (cr, x, y);

        // segments
        for (int j=0; j<nn; j++) {
            double r = rr * drand48() / 8.0;

            double angle1 = j * step + delta;
            double x1 = cp + r * cos(angle1);
            double y1 = cp + r * sin(angle1);

            double x2 = cp + rr * cos(angle1);
            double y2 = cp + rr * sin(angle1);

            cairo_curve_to (ctx->cairo_cr,
                            x1, y1,
                            x2, y2, x2,y2);
        }
        cairo_close_path(cr);
        cairoRandomizeColor(ctx);

        // TODO: apply other fill types
        if (ctx->shape_defaults.fill_type == CB_FILL_SOLID) {
            cairo_fill (cr);
        }
        cairo_stroke (cr);

    }
    delta += 0.5;
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
