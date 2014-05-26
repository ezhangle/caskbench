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

#include "forward.h"
#include "caskbench.h"
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


void (*cairoShapes[CB_SHAPE_END-1])(caskbench_context_t *ctx , shapes_t *args) = {
    cairoDrawCircle,
    cairoDrawRectangle,
    cairoDrawTriangle,
    cairoDrawStar,
    cairoDrawRoundedRectangle
};

void
cairoDrawRandomizedShape(caskbench_context_t *ctx, shapes_t *shape)
{
    cairo_pattern_t *pattern = NULL;
    cairo_surface_t *image = NULL;

    static const double dashed3[] = {1.0};
    static const double dashed2[] = {14.0, 6.0};

    // Shape Type
    if (shape->shape_type == CB_SHAPE_NONE)
        shape->shape_type = generate_random_shape();

    // Stroke styles
    if (shape->stroke_width)
    {
        cairo_set_line_width(ctx->cairo_cr, shape->stroke_width);
        cairo_set_line_join(ctx->cairo_cr,  (cairo_line_join_t)shape->join_style);
        cairo_set_line_cap(ctx->cairo_cr, (cairo_line_cap_t)shape->cap_style);
        if (shape->dash_style == 1)
            cairo_set_dash(ctx->cairo_cr, dashed3, 1, 0);
        else if (shape->dash_style == 2)
            cairo_set_dash(ctx->cairo_cr, dashed2, 2, 0);
    }

    // Options for fill, gradient, and transparency
    switch (shape->fill_type) {
        case CB_FILL_IMAGE_PATTERN:
            // TODO
            break;
        case CB_FILL_RADIAL_GRADIENT:
            // TODO
            break;
        case CB_FILL_LINEAR_GRADIENT:
            // TODO
            break;
        case CB_FILL_NONE:
        case CB_FILL_SOLID:
        default:
            if (shape->red > 0 || shape->blue > 0 || shape->green > 0 || shape->alpha > 0)
            {
                cairo_set_source_rgba(ctx->cairo_cr,
                                      shape->red ? (double)shape->red : 0.0,
                                      shape->green ? (double)shape->green : 0.0,
                                      shape->blue ? (double)shape->blue : 0.0,
                                      shape->alpha ? (double)shape->alpha : 1.0 );
            }
            else {
                cairoRandomizeColor(ctx);
            }
            break;
    }

    // Draw
    cairoShapes[shape->shape_type] (ctx, shape);

    // Cleanup
    if (pattern != NULL)
        cairo_pattern_destroy(pattern);
    if (image != NULL)
        cairo_surface_destroy (image);
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
