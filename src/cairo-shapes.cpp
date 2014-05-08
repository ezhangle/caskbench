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
#include <math.h>

#include "cairo-shapes.h"


void
cairoRandomizeColor(caskbench_context_t *ctx)
{
    double red, green, blue, alpha;

    red = (double)rand()/RAND_MAX;
    green = (double)rand()/RAND_MAX;
    blue = (double)rand()/RAND_MAX;
    alpha = (double)rand()/RAND_MAX;

    cairo_set_source_rgba (ctx->cairo_cr, red, green, blue, alpha);
}

void cairoDrawCircle(caskbench_context_t *ctx, shapes_t *args)
{
    cairo_arc (ctx->cairo_cr,
               args->x + args->radius,
               args->y + args->radius,
               args->radius, 0, 2*M_PI);
}

void
cairoDrawRectangle(caskbench_context_t *ctx, shapes_t *args)
{
    cairo_rectangle (ctx->cairo_cr, args->x, args->y, args->width, args->height);
}


void
cairoDrawTriangle(caskbench_context_t *ctx, shapes_t *args)
{
    cairo_move_to (ctx->cairo_cr, args->x, args->y+2*args->radius);
    cairo_rel_line_to (ctx->cairo_cr, 2*args->radius, 0);
    cairo_rel_line_to (ctx->cairo_cr, -args->radius, -2*args->radius);
    cairo_close_path(ctx->cairo_cr);
}


void
cairoDrawStar(caskbench_context_t *ctx, shapes_t *args)
{
    int px = args->x + 2*args->radius * star_points[0][0]/200.0;
    int py = args->y + 2*args->radius * star_points[0][1]/200.0;
    cairo_move_to (ctx->cairo_cr, px, py);

    for (int p = 1; p < 10; p++ ) {
        px = args->x + 2*args->radius * star_points[p][0]/200.0;
        py = args->y + 2*args->radius * star_points[p][1]/200.0;
        cairo_line_to (ctx->cairo_cr, px, py);
    }
    cairo_close_path(ctx->cairo_cr);
}


void
cairoDrawRoundedRectangle (caskbench_context_t *ctx, shapes_t *args)
{
    double h = args->height;
    double w = args->width;
    double x = args->x;
    double y = args->y;
    double r = args->radius;
    cairo_move_to (ctx->cairo_cr, x + r, y);
    cairo_arc (ctx->cairo_cr, x + w - r, y + r,     r, M_PI + M_PI / 2, M_PI * 2        );
    cairo_arc (ctx->cairo_cr, x + w - r, y + h - r, r, 0,               M_PI / 2        );
    cairo_arc (ctx->cairo_cr, x + r,     y + h - r, r, M_PI/2,          M_PI            );
    cairo_arc (ctx->cairo_cr, x + r,     y + r,     r, M_PI,            270 * M_PI / 180);
}


void (*cairoShapes[5])(caskbench_context_t *ctx , shapes_t *args) = {
    cairoDrawCircle,
    cairoDrawRectangle,
    cairoDrawTriangle,
    cairoDrawStar,
    cairoDrawRoundedRectangle
};

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
