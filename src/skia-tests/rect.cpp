/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <math.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"

/* TODO:
 * - Specify with or without alpha
 * - With or without antialiasing?
 * - Stroke width should be via the shape defaults
 */

int
sk_setup_rect(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(false);
    ctx->skia_paint->setStrokeWidth(1);

    return 1;
}

void
sk_teardown_rect(void)
{
}

int
sk_test_rect(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;

    shapes_t shape;
    shape_copy(&ctx->shape_defaults, &shape);
    for (int i=0; i<ctx->size; i++) {
        double x1 = (double)rand()/RAND_MAX * w;
        double x2 = (double)rand()/RAND_MAX * w;
        double y1 = (double)rand()/RAND_MAX * h;
        double y2 = (double)rand()/RAND_MAX * h;

        shape.x = MIN(x1, x2);
        shape.y = MIN(x1, x2);
        shape.width = abs(x2 - x1);
        shape.height = abs(y2 - y1);

        if (ctx->shape_defaults.fill_type == CB_FILL_RANDOM) {
            shape.fill_type = generate_random_fill_type();
        }
        sk_set_fill_style(ctx, &shape);

        shape.shape_type = CB_SHAPE_RECTANGLE;
        skiaDrawRandomizedShape(ctx,&shape);
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
