/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <math.h>
#include <cairo.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"

static cairo_surface_t *image;
static cairo_surface_t *cached_image;
int
ca_setup_clip(caskbench_context_t *ctx)
{
    image = cairoCreateSampleImage (ctx);
    cached_image = cairoCacheImageSurface (ctx, image);
    return 1;
}

void
ca_teardown_clip(void)
{
    cairo_surface_destroy (image);
    cairo_surface_destroy (cached_image);
}

#if 1
// Single static star and random clip
int
ca_test_clip(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    double ratio;
    ratio = (double) w/h;
    shapes_t shape;
    double scale_param_x = w/80;
    double scale_param_y;
    cairo_t *cr = ctx->cairo_cr;

    shape_copy(&ctx->shape_defaults, &shape);
    cairo_new_path(cr);
    cairo_save (cr);

    /* Draw star for the full screen size */
    if (ratio > 1.0)
        scale_param_y = scale_param_x * (1/ratio);
    else if (ratio < 1.0)
        scale_param_y = scale_param_x + (ratio);
    else
        scale_param_y = scale_param_x;

    cairo_scale (cr, scale_param_x,scale_param_y);
    shape.x = 0;
    shape.y = 0;
    shape.radius = 40;
    shape.shape_type = CB_SHAPE_STAR;
    shape.fill_type = CB_FILL_SOLID;

    ca_set_fill_style(ctx, &shape);
    cairoDrawRandomizedShape(ctx,&shape);

    for (int i=0; i<ctx->size; i++) {
        cairoDrawStar(ctx,&shape);
        cairo_clip (cr);
        double x1 = (double)rand()/RAND_MAX * w;
        double x2 = (double)rand()/RAND_MAX * w;
        double y1 = (double)rand()/RAND_MAX * h;
        double y2 = (double)rand()/RAND_MAX * h;
        cairo_set_source_surface (cr, cached_image, x1, y1);
        // To scale without blur
        cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_FAST);
        cairo_paint (cr);
        cairo_reset_clip(cr);
    }

    cairo_restore (cr);

    return 1;
}
#else
// Random stars and random clip
int
ca_test_clip(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;

    shapes_t shape;
    shape_copy(&ctx->shape_defaults, &shape);
    cairo_set_source_rgb (ctx->cairo_cr, 0, 0, 0);
    cairo_paint (ctx->cairo_cr);

    cairo_t *cr = ctx->cairo_cr;
    for (int i=0; i<ctx->size; i++) {
        double i = (double)rand()/RAND_MAX * w;
        double j = (double)rand()/RAND_MAX * h;
        double x1 = (double)rand()/RAND_MAX * 80;
        double y1 = (double)rand()/RAND_MAX * 80;

        cairo_new_path(cr);
        cairo_save (cr);
        cairo_translate (cr,i,j);
        cairo_scale(cr,0.5,0.5);

        shape.x = 0;
        shape.y = 0;
        shape.radius = 40;
        shape.shape_type = CB_SHAPE_STAR;
        shape.fill_type = CB_FILL_SOLID;
        ca_set_fill_style(ctx, &shape);
        cairoDrawRandomizedShape(ctx,&shape);

        cairoDrawStar(ctx,&shape);
        cairo_clip(cr);
        cairo_set_source_surface (cr, image, x1, y1);
        cairo_paint (cr);
        cairo_reset_clip(cr);
        cairo_restore(cr);
    }

    return 1;
}
#endif
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
