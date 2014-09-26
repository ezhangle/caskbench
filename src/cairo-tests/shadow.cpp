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

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"
#include "kinetics.h"

static double line_width;

static double shadow_x_blur;
static double shadow_y_blur;

static double spread_line_width;
static double spread_x_blur;
static double spread_y_blur;

static double drop_x_blur;
static double drop_y_blur;
static double drop_x_offset;
static double drop_y_offset;
static double drop_line_width;

static double x_offset;
static double y_offset;

static kinetics_t *particles;

#ifdef USE_CAIROGLES
static void particle_init ()
{
    line_width = 20.0;

    /* Inset shadow */
    shadow_x_blur = 5;
    shadow_y_blur = 2;

    x_offset = 6;
    y_offset = 1;

    /* Spread */
    spread_x_blur = 5;
    spread_y_blur = 2;
    spread_line_width = 3;

    /* Drop shadow */
    drop_x_blur = 10;
    drop_y_blur = 8;
    drop_x_offset = -30;
    drop_y_offset = -5;
    drop_line_width = line_width;
}

static void fill_particle (caskbench_context_t *ctx, shapes_t *shape)
{
    cairo_t *cr = ctx->cairo_cr;
    /* Drop shadow */
    cairo_set_shadow (cr, CAIRO_SHADOW_DROP);
    cairo_set_draw_shadow_only (cr, 1);
    cairo_set_shadow_rgba (cr, 0, 0, 0, 0.5);
    cairo_set_shadow_offset (cr, drop_x_offset,
				 drop_y_offset);
    cairo_set_shadow_blur (cr, drop_x_blur,
			       drop_y_blur);
    cairoDrawCircle (ctx, shape);
    cairo_set_line_width (cr, drop_line_width);
    cairo_stroke (cr);
    /* Ring */
    cairoRandomizeColor (ctx);
    cairo_set_shadow (cr, CAIRO_SHADOW_INSET);
    cairo_set_draw_shadow_only (cr, 0);

    // cairo_set_shadow_rgba (cr, 0, 0, 0, 1); /* Disable shadow masking to make output similar to skia */
    cairo_set_shadow_offset (cr, x_offset,
				 y_offset);
    cairo_set_shadow_blur (cr, shadow_x_blur,
			       shadow_y_blur);
    cairoDrawCircle (ctx,shape);
    cairo_set_line_width (cr, line_width);
    cairo_stroke (cr);

    /* Spread */
    cairo_set_shadow (cr, CAIRO_SHADOW_DROP);
    cairo_set_draw_shadow_only (cr, 1);
    cairo_set_shadow_rgba (cr, 1, 1, 1, 1);
    cairo_set_shadow_offset (cr, x_offset,
				 y_offset);
    cairo_set_shadow_blur (cr, spread_x_blur,
			       spread_y_blur);
    cairoDrawCircle (ctx,shape);
    cairo_set_line_width (cr, spread_line_width);
    cairo_stroke (cr);
}
#endif

int
ca_setup_shadow(caskbench_context_t *ctx)
{
#ifndef USE_CAIROGLES
    return 0;
#else
    if (ctx->size < 0)
        return 0;
    // Animation & Shadow setup
    particles = (kinetics_t *) malloc (sizeof (kinetics_t) * ctx->size);
    for (int i = 0; i < ctx->size; i++)
        kinetics_init(&particles[i], ctx);

    particle_init ();
    return 1;
#endif
}

void
ca_teardown_shadow(void)
{
#ifndef USE_CAIROGLES
    return;
#else
    free(particles);
#endif
}

int
ca_test_shadow(caskbench_context_t *ctx)
{
#ifndef USE_CAIROGLES
    return 0;
#else
    cairo_set_source_rgb (ctx->cairo_cr, 1, 1, 1);
    cairo_rectangle (ctx->cairo_cr, 0, 0, ctx->canvas_width ,ctx->canvas_height);
    cairo_fill (ctx->cairo_cr);

    for (int i = 0; i < ctx->size; i++) {
        shapes_t shape;
        kinetics_t *particle = &particles[i];
        kinetics_update(particle, 0.1);

        shape_copy(&ctx->shape_defaults, &shape);
        shape.width = particle->width;
        shape.height = particle->height;
        shape.x = particle->x;
        shape.y = particle->y;
        if (!shape.radius)
            shape.radius = 40;

        cairo_shadow_enable_cache (ctx->cairo_cr, 1);
        fill_particle (ctx, &shape);
    }
    return 1;
#endif
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
