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
sk_setup_lines(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(false);
    ctx->skia_paint->setStrokeWidth(1);
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    return 1;
}

void
sk_teardown_lines(void)
{
}

int
sk_test_lines(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    double x = (double)rand()/RAND_MAX * w;
    double y = (double)rand()/RAND_MAX * h;
    SkPath path;

    path.moveTo(x, y);
    for (int i=0; i<ctx->size; i++) {
        x = (double)rand()/RAND_MAX * w;
        y = (double)rand()/RAND_MAX * h;

        path.lineTo(x, y);
    }

    skiaRandomizePaintColor(ctx);
    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));

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
