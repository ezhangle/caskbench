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
#include "caskbench_context.h"
#include "cairo-shapes.h"
bool do_stroke = false; bool do_fill = false;

cairo_pattern_t*
cairoCreateRadialGradientPattern (const shapes_t *shape)
{
    double radius = shape->radius;
    double center_x, center_y;

    if (shape->radius > 0) {
        center_x = shape->x + shape->radius;
        center_y = shape->y + shape->radius;
    } else if (shape->width > shape->height) {
        radius = shape->height/2;
        center_x = shape->x + radius + ((double)rand()*shape->width)/RAND_MAX;
        center_y = shape->y + radius;
    } else {
        radius = shape->width/2;
        center_x = shape->x + radius;
        center_y = shape->y + radius + ((double)rand()*shape->width)/RAND_MAX;
    }

    double red, green, blue, alpha;
    generate_random_color(red,green,blue,alpha);
    cairo_pattern_t *pattern;
    pattern = cairo_pattern_create_radial (center_x, center_y, radius, center_x, center_y, 0);
    cairo_pattern_add_color_stop_rgba (pattern, 0, red, green, blue, alpha);
    generate_random_color(red,green,blue,alpha);
    cairo_pattern_add_color_stop_rgba (pattern, 1, red, green, blue, alpha);
    return pattern;
}

cairo_pattern_t*
cairoCreateLinearGradientPattern (int y1, int y2)
{
    double red, green, blue, alpha;

    cairo_pattern_t *pattern;
    /* cairo_pattern_set_extend() has been removed as it causes incorrect outputs */
    pattern = cairo_pattern_create_linear (0, y1, 0, y2);
    generate_random_color(red,green,blue,alpha);
    cairo_pattern_add_color_stop_rgba (pattern, 0, red, green, blue, alpha);
    generate_random_color(red,green,blue,alpha);
    cairo_pattern_add_color_stop_rgba (pattern, 1, red, green, blue, alpha);
    return pattern;
}

cairo_pattern_t*
cairoCreateBitmapPattern (const char *image_path)
{
    cairo_matrix_t   matrix;
    cairo_surface_t *image = NULL;
    cairo_pattern_t *pattern;
    image = cairo_image_surface_create_from_png (image_path);
    pattern = cairo_pattern_create_for_surface (image);
    return pattern;
}

void
cairoRandomizeColor(caskbench_context_t *ctx)
{
    double red, green, blue, alpha;
    generate_random_color(red,green,blue,alpha);
    cairo_set_source_rgba (ctx->cairo_cr, red, green, blue, alpha);
}

void
ca_set_fill_style(caskbench_context_t *ctx, const shapes_t *shape)
{
    cairo_pattern_t* pattern = NULL;
    switch (shape->fill_type) {
        case CB_FILL_NONE:
            do_stroke = true;
            break;
        case CB_FILL_SOLID:
            if (shape->stroke_width <= 0)
                do_fill = true;
            else {
                do_fill = true;
                do_stroke = true;
            }
            break;
        case CB_FILL_LINEAR_GRADIENT:
            if(shape->shape_type == CB_SHAPE_STAR || shape->shape_type == CB_SHAPE_CIRCLE)
                pattern = cairoCreateLinearGradientPattern(shape->y-shape->radius, shape->y+shape->radius);
            else
                pattern = cairoCreateLinearGradientPattern(shape->y, shape->y + shape->height);
            cairo_set_source (ctx->cairo_cr, pattern);
            do_fill = true;
            break;
        case CB_FILL_RADIAL_GRADIENT:
            pattern = cairoCreateRadialGradientPattern(shape);
            cairo_set_source (ctx->cairo_cr, pattern);
            do_fill = true;
            break;
        case CB_FILL_IMAGE_PATTERN:
            if(ctx->stock_image_path) {
                pattern = cairoCreateBitmapPattern(ctx->stock_image_path);
                cairo_set_source (ctx->cairo_cr, pattern);
            }
            do_fill = true;
            break;
#if 0
        case CB_FILL_HERRINGBONE_PATTERN:  /* TODO */
            break;
#endif
        default:
            break;
    }

    if (pattern)
        cairo_pattern_destroy (pattern);
}

void
cairoDrawLine(caskbench_context_t *ctx, shapes_t *args)
{
    cairo_move_to (ctx->cairo_cr, args->x, args->y);
    cairo_line_to (ctx->cairo_cr, args->x + args->width, args->y + args->height);
}

void
cairoDrawQuadraticCurve(caskbench_context_t *ctx, shapes_t *args)
{
    double qx1 = args->dx1 * 2.0 / 3.0;
    double qy1 = args->dy1 * 2.0 / 3.0;

    double qx2 = args->width - args->dx1 * 2.0 / 3.0;
    double qy2 = args->height - args->dy1 * 2.0 / 3.0;

    cairo_move_to (ctx->cairo_cr, args->x, args->y);
#if 0
    cairo_rel_curve_to (ctx->cairo_cr,
                        qx1, qy1,
                        qx2, qy2,
                        args->width, args->height);
#else
    cairo_rel_curve_to (ctx->cairo_cr,
                        args->dx1, args->dy1,
                        args->width, args->height,
                        args->width, args->height);
#endif
}

void
cairoDrawCubicCurve(caskbench_context_t *ctx, shapes_t *args)
{
    cairo_move_to (ctx->cairo_cr, args->x, args->y);
    cairo_rel_curve_to (ctx->cairo_cr,
                        args->dx1, args->dy1,
                        args->dx2, args->dy2,
                        args->width, args->height);
}

void
cairoDrawCircle(caskbench_context_t *ctx, shapes_t *args)
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
    double r = 4.0; /* To make the output in sync with skia */

#if USE_CAIROGLES
    cairo_rounded_rectangle (cr, x, y, w, h, r, r, r, r);
#else
    cairo_move_to (ctx->cairo_cr, x + r, y);
    cairo_arc (ctx->cairo_cr, x + w - r, y + r,     r, M_PI + M_PI / 2, M_PI * 2        );
    cairo_arc (ctx->cairo_cr, x + w - r, y + h - r, r, 0,               M_PI / 2        );
    cairo_arc (ctx->cairo_cr, x + r,     y + h - r, r, M_PI/2,          M_PI            );
    cairo_arc (ctx->cairo_cr, x + r,     y + r,     r, M_PI,            270 * M_PI / 180);
#endif
}

cairo_surface_t *cairoCreateSampleImage (caskbench_context_t *ctx)
{
    cairo_surface_t *image;
    cairo_t *cr_image;
    int i, x, y;
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
    return image;
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
    static const double dashed3[] = {1.0};
    static const double dashed2[] = {14.0, 6.0};

    cairo_path_t *new_path;

    // Shape Type
    if (shape->shape_type == CB_SHAPE_RANDOM)
        shape->shape_type = generate_random_shape();

    // Options for fill, gradient and transparency
    if (shape->fill_type == CB_FILL_RANDOM) {
        shape->fill_type = generate_random_fill_type();
    }

    ca_set_fill_style(ctx, shape);

    if ((shape->fill_type == CB_FILL_NONE) || (shape->fill_type == CB_FILL_SOLID)) {
        if (shape->fill_color != -1) {
            cairo_set_source_rgba (ctx->cairo_cr,
                    ((shape->fill_color >> 24) & 255) / 255.0,
                    ((shape->fill_color >> 16) & 255) / 255.0,
                    ((shape->fill_color >> 8) & 255) / 255.0,
                    ((shape->fill_color) & 255) / 255.0);
        }
        else {
            cairoRandomizeColor(ctx);
        }
    }

     // Draw
    cairoShapes[shape->shape_type-1] (ctx, shape);
    new_path = cairo_copy_path(ctx->cairo_cr);
    if (do_fill) {
        cairo_fill (ctx->cairo_cr);
    }
    if (do_stroke && !shape->stroke_width) {
        cairo_new_path (ctx->cairo_cr);
        cairo_append_path (ctx->cairo_cr, new_path);
        cairo_stroke (ctx->cairo_cr);
    }
    do_stroke = do_fill = false;
    // Stroke styles
    if (shape->stroke_width)
    {
        cairo_new_path (ctx->cairo_cr);
        cairo_append_path (ctx->cairo_cr, new_path);

        cairo_set_line_width (ctx->cairo_cr, shape->stroke_width);
        cairo_set_line_join (ctx->cairo_cr,  (cairo_line_join_t)shape->join_style);
        cairo_set_line_cap (ctx->cairo_cr, (cairo_line_cap_t)shape->cap_style);
        if (shape->dash_style == 0)
            cairo_set_dash (ctx->cairo_cr, dashed3, 1, 0);
        else if (shape->dash_style == 1)
            cairo_set_dash (ctx->cairo_cr, dashed2, 2, 0);

        if (shape->stroke_color != -1) {
            cairo_set_source_rgb (ctx->cairo_cr,
                                  ((shape->stroke_color >> 24) & 255) / 255.0,
                                  ((shape->stroke_color >> 16) & 255) / 255.0,
                                  ((shape->stroke_color >> 8) & 255) / 255.0);
        }
        else {
            cairoRandomizeColor (ctx);
        }

        cairo_stroke (ctx->cairo_cr);
    }

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
