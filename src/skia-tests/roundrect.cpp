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

int
sk_setup_roundrect(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(true);
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    return 1;
}

void
sk_teardown_roundrect(void)
{
}

int
sk_test_roundrect(caskbench_context_t *ctx)
{
    int i;
    double line_width, x, y, radius;

    for (i=0; i<ctx->size; i++) {
        shapes_t shape;
        shape_copy(&ctx->shape_defaults, &shape);

        skiaRandomizePaintColor(ctx);

        shape.x = trunc( (((double)ctx->canvas_width-20)*rnd())/RAND_MAX ) + 10;
        shape.y = trunc( (((double)ctx->canvas_height-20)*rnd())/RAND_MAX ) + 10;

        /* vary radius upto half of MIN(X,Y) */
        shape.radius = (double)rnd()/RAND_MAX * 20;
        shape.width = 100;
        shape.height = 40;

        /* line_width cannot be more than twice of radius due to skia limitation - Issue #4 in skia https://github.com/Samsung/skia/issues/4 */
        shape.stroke_width = (double)rnd()/RAND_MAX * (2*shape.radius);

        ctx->skia_paint->setStrokeWidth(shape.stroke_width);

        skiaDrawRoundedRectangle (ctx, &shape);
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
