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
    SkRect rect;

    for (i=0; i<ctx->size; i++) {
        skiaRandomizePaintColor(ctx);

        x = trunc( (((double)ctx->canvas_width-20)*rand())/RAND_MAX ) + 10;
        y = trunc( (((double)ctx->canvas_height-20)*rand())/RAND_MAX ) + 10;
        rect.set(x, y, x+100, y+40);

        /* vary radius upto half of MIN(X,Y) */
        radius = (double)rand()/RAND_MAX * 20;

        /* line_width cannot be more than twice of radius due to skia limitation - Issue #4 in skia https://github.com/Samsung/skia/issues/4 */
        line_width = (double)rand()/RAND_MAX * (2*radius);
        ctx->skia_paint->setStrokeWidth(line_width);
        ctx->skia_canvas->drawRoundRect(rect, radius, radius, *(ctx->skia_paint));
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
