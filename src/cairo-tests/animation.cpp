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

static kinetics_t *particles;

int
ca_setup_animation(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;

    // Animation setup
    particles = (kinetics_t *) malloc (sizeof (kinetics_t) * ctx->size);
    for (int i = 0; i < ctx->size; i++)
        kinetics_init(&particles[i], ctx);

    return 1;
}

void
ca_teardown_animation(void)
{
    free(particles);
}

int
ca_test_animation(caskbench_context_t *ctx)
{
    // Animation / Kinematics of single or multi shape
    cairo_set_source_rgb (ctx->cairo_cr, 0, 0, 0);
    cairo_rectangle (ctx->cairo_cr, 0, 0, ctx->canvas_width ,ctx->canvas_height);
    cairo_fill (ctx->cairo_cr);

    static int counter = 0;
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

        cairo_save(ctx->cairo_cr);
        cairo_rotate(ctx->cairo_cr, counter/50.0);

        cairoDrawRandomizedShape(ctx, &shape);
        cairo_restore(ctx->cairo_cr);

        counter++;
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
