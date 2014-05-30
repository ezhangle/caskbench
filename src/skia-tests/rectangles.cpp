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
sk_setup_rectangles(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(false);
    ctx->skia_paint->setStrokeWidth(1);
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    return 1;
}

void
sk_teardown_rectangles(void)
{
}

int
sk_test_rectangles(caskbench_context_t *ctx)
{
    int i, x, y, w, h;
    SkRect rect;

    for (i=0; i<ctx->size; i++) {
        skiaRandomizePaintColor(ctx);
        w = trunc( (0.5*(double)ctx->canvas_width*rand())/RAND_MAX ) + 1;
        h = trunc( (0.5*(double)ctx->canvas_height*rand())/RAND_MAX ) + 1;
        x = trunc( (0.5*(double)ctx->canvas_width*rand())/RAND_MAX );
        y = trunc( (0.5*(double)ctx->canvas_height*rand())/RAND_MAX );
        rect.set(x, y, x+w, y+h);

        ctx->skia_canvas->drawRect(rect, *(ctx->skia_paint));
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
