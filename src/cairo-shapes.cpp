#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>

#include "cairo-shapes.h"
static int star_points[11][2] = {
    { 0, 85 },
    { 75, 75 },
    { 100, 10 },
    { 125, 75 },
    { 200, 85 },
    { 150, 125 },
    { 160, 190 },
    { 100, 150 },
    { 40, 190 },
    { 50, 125 },
    { 0, 85 }
};

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
    cairo_arc (ctx->cairo_cr, args->centre_x, args->centre_y, args->radius, 0, 2*M_PI);
}

void cairoDrawRectangle(caskbench_context_t *ctx, shapes_t *args)
{
    cairo_rectangle (ctx->cairo_cr, args->centre_x, args->centre_y, args->width, args->height);
}

void cairoDrawTriangle(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->shape_args.numpoints = 3;
    ctx->shape_args.points = (double (*)[2]) malloc(ctx->shape_args.numpoints*2*(sizeof(double)));
    ctx->shape_args.points[0][0] = ctx->shape_args.centre_x;
    ctx->shape_args.points[0][1] = ctx->shape_args.centre_y+2*ctx->shape_args.radius;
    ctx->shape_args.points[1][0] = 2*ctx->shape_args.radius;
    ctx->shape_args.points[1][1] = 0;
    ctx->shape_args.points[2][0] = -ctx->shape_args.radius;
    ctx->shape_args.points[2][1] = -2*ctx->shape_args.radius;


    for (int p = 0; p < 3; p++ ) {
        if(p == 0)
            cairo_move_to (ctx->cairo_cr, args->points[p][0], args->points[0][1]);
        else
            cairo_rel_line_to (ctx->cairo_cr, args->points[p][0], args->points[p][1]);
    }
    free (ctx->shape_args.points);
}

void cairoDrawStar(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->shape_args.numpoints = 10;
    ctx->shape_args.points = (double (*)[2]) malloc(ctx->shape_args.numpoints*2*(sizeof(double)));

    for (int p = 0; p < 10; p++ ) {
        int px = ctx->shape_args.centre_x + 2*ctx->shape_args.radius * star_points[p][0]/200.0;
        int py = ctx->shape_args.centre_y + 2*ctx->shape_args.radius * star_points[p][1]/200.0;
        ctx->shape_args.points[p][0] = px;
        ctx->shape_args.points[p][1] = py;
    }

    for (int p = 0; p < args->numpoints; p++ ) {
        if(p == 0)
            cairo_move_to (ctx->cairo_cr, args->points[p][0], args->points[0][1]);
        else
            cairo_line_to (ctx->cairo_cr, args->points[p][0], args->points[p][1]);
    }
    cairo_close_path(ctx->cairo_cr);
    free (ctx->shape_args.points);
}


void
cairoDrawRoundedRectangle (caskbench_context_t *ctx, shapes_t *args)
{
    double h = args->height;
    double w = args->width;
    double x = args->centre_x;
    double y = args->centre_y;
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
