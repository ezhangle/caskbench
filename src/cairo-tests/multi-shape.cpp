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

static int element_spacing;
static int num_x_elements;
static int num_y_elements;

int
ca_setup_multi_shape(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;

    // Multi-shape setup
    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    return 1;
}

void
ca_teardown_multi_shape(void)
{
}

int
ca_test_multi_shape(caskbench_context_t *ctx)
{
    ctx->shape_defaults.radius = 0.9 * element_spacing / 2;
    ctx->shape_defaults.width = 2*ctx->shape_defaults.radius;
    ctx->shape_defaults.height = 2*ctx->shape_defaults.radius;

    for (int j=0; j<num_y_elements; j++) {
        for (int i=0; i<num_x_elements; i++) {
            shapes_t shape;
            shape_copy(&ctx->shape_defaults, &shape);
            shape.x = i * element_spacing;
            shape.y = j * element_spacing;

            cairoDrawRandomizedShape(ctx, &shape);
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
