/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <SkCanvas.h>
#include <effects/SkGradientShader.h>

#include "forward.h"
#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"
#include "kinetics.h"

static kinetics_t *skia_particles;
static int element_spacing;
static int num_x_elements;
static int num_y_elements;

int
sk_setup_fill(caskbench_context_t *ctx)
{
    if (ctx->size <= 0)
        return 0;

    return 1;
}

void
sk_teardown_fill(void)
{
}

int
sk_test_fill(caskbench_context_t *ctx)
{
    for (int i = 0; i < ctx->size; i++)
    {
        shapes_t shape;
        shape_copy(&ctx->shape_defaults, &shape);

        if (!(shape.x && shape.y))
        {
            shape.x = ctx->canvas_width/2;
            shape.y = ctx->canvas_height/2;
        }
        if (!(shape.width && shape.height))
        {
            shape.width = 100;
            shape.height = 50;
        }
        if (!shape.radius)
            shape.radius = 40;

        /* Height is used for creating patterns, needs to be updated for star and triangle */
        if (shape.shape_type == CB_SHAPE_STAR || shape.shape_type == CB_SHAPE_TRIANGLE)
            shape.height = 2*shape.radius;

        /* Use rectangle as default shape */
        shape.shape_type = shape.shape_type ? shape.shape_type:CB_SHAPE_RECTANGLE;
        skiaDrawRandomizedShape(ctx, &shape);
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
