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
sk_setup_transform(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(false);
    ctx->skia_paint->setStrokeWidth(1);
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    return 1;
}

void
sk_teardown_transform(void)
{
}

int
sk_test_transform(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    SkScalar rotation_delta(0.02*180/M_PI);

    ctx->skia_canvas->save();
    for (int nn=0; nn<ctx->size; nn++) {
        for (int i=0; i<w; i+=50) {
            SkPath p1;
            p1.moveTo(i, 0);
            p1.lineTo(i, h);
            skiaRandomizePaintColor(ctx);
            ctx->skia_canvas->drawPath(p1, *(ctx->skia_paint));
        }
        for (int j=0; j<h; j+=50) {
            SkPath p2;
            p2.moveTo(0, j);
            p2.lineTo(w, j);
            skiaRandomizePaintColor(ctx);
            ctx->skia_canvas->drawPath(p2, *(ctx->skia_paint));
        }
        ctx->skia_canvas->translate(w/2.0, h/2.0);
        ctx->skia_canvas->rotate(rotation_delta);
        ctx->skia_canvas->scale(1.004, 0.996);
        ctx->skia_canvas->translate(-w/2.0, -h/2.0);
    }
    ctx->skia_canvas->restore();

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
