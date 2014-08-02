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

static double delta = 0;

int
sk_setup_curves(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(false);
    ctx->skia_paint->setStrokeWidth(1);
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    return 1;
}

void
sk_teardown_curves(void)
{
}

int
sk_test_curves(caskbench_context_t *ctx)
{
    SkPath path;
    int cp = ctx->canvas_width / 2; // center point
    int rr = ctx->canvas_width / 2;
    int nn = 32; // must be even
    double step = (2 * M_PI) / nn;


    for (int i=0; i<ctx->size; i++) {
        double angle = delta;  // in radians
        double x = cp + rr * cos(angle);
        double y = cp + rr * sin(angle);


        // begin path
        SkPath path;
        path.moveTo(SkDoubleToScalar(x), SkDoubleToScalar(y));

        // segments
        for (int j=0; j<nn; j++) {
            double r = rr * (double)rand()/RAND_MAX / 8.0;

            double angle = j * step + delta;

            double x1 = cp + r * cos(angle);
            double y1 = cp + r * sin(angle);
            double x2 = cp + rr * cos(angle);
            double y2 = cp + rr * sin(angle);

            path.quadTo(SkDoubleToScalar(x1), SkDoubleToScalar(y1),
                         SkDoubleToScalar(x2), SkDoubleToScalar(y2));
        }

        skiaRandomizePaintColor(ctx);

        // TODO: apply other fill types
        if (ctx->shape_defaults.fill_type == CB_FILL_SOLID) {
            ctx->skia_paint->setStyle(SkPaint::kStrokeAndFill_Style);
        }

        path.close();
        ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));
    }
    delta += 0.5;

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
