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

    // Animation setup
    skia_particles = (kinetics_t *) malloc (sizeof (kinetics_t) * ctx->size);
    for (int i = 0; i < ctx->size; i++)
        kinetics_init(&skia_particles[i]);

    // Multi-shape setup
    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    return 1;
}

void
sk_teardown_fill(void)
{
    free(skia_particles);
}

int
sk_test_fill(caskbench_context_t *ctx)
{
    // Animation / Kinematics of single or multi shape
    if (ctx->shape_defaults.animation) {
        ctx->skia_canvas->drawColor(SK_ColorBLACK);

        for (int i = 0; i < ctx->size; i++) {
            shapes_t shape;
            kinetics_t *particle = &skia_particles[i];

            kinetics_update(particle, 0.1);

            shape_copy(&ctx->shape_defaults, &shape);
            shape.width = particle->width;
            shape.height = particle->height;
            shape.x = particle->x;
            shape.y = particle->y;

            skiaDrawRandomizedShape(ctx, &shape);
        }
    }

    // Drawing of multishape on a grid
    else if (ctx->shape_defaults.multi_shapes) {
        ctx->shape_defaults.radius = 0.9 * element_spacing / 2;
        ctx->shape_defaults.width = 2*ctx->shape_defaults.radius;
        ctx->shape_defaults.height = 2*ctx->shape_defaults.radius;

        for (int j=0; j<num_y_elements; j++) {
            for (int i=0; i<num_x_elements; i++) {
                shapes_t shape;
                shape_copy(&ctx->shape_defaults, &shape);
                shape.x = i * element_spacing;
                shape.y = j * element_spacing;

                skiaDrawRandomizedShape(ctx, &shape);
            }
        }
    }

    else {
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

            skiaDrawRandomizedShape(ctx, &shape);
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
