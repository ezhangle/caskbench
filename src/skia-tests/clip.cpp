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

#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"

static SkBitmap bitmap;

int
sk_setup_clip(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(false);
    bitmap = skiaCreateSampleImage (ctx);
    return 1;
}

void
sk_teardown_clip(void)
{
}

#if 1
// Single static star and random clip
int
sk_test_clip(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;

    double ratio;
    ratio = (double) w/h;
    shapes_t shape;
    double scale_param_x = w/80;
    double scale_param_y;

    shape_copy(&ctx->shape_defaults, &shape);
    ctx->skia_canvas->save();

    /* Draw star for the full screen size */
    if (ratio > 1.0)
        scale_param_y = scale_param_x * (1/ratio);
    else if (ratio < 1.0)
        scale_param_y = scale_param_x + (ratio);
    else
        scale_param_y = scale_param_x;
    ctx->skia_canvas->scale(scale_param_x,scale_param_y);

    shape.x = 0;
    shape.y = 0;
    shape.radius = 40;
    shape.shape_type = CB_SHAPE_STAR;
    shape.fill_type = CB_FILL_SOLID;

    sk_set_fill_style(ctx, &shape);
    skiaDrawRandomizedShape(ctx, &shape);

    for (int i=0; i<ctx->size; i++) {
        ctx->skia_canvas->clipPath(getCurrentSkiaPath(), SkRegion::kReplace_Op, true);
        double x1 = (double)rnd()/RAND_MAX * w;
        double x2 = (double)rnd()/RAND_MAX * w;
        double y1 = (double)rnd()/RAND_MAX * h;
        double y2 = (double)rnd()/RAND_MAX * h;
        ctx->skia_canvas->drawBitmap(bitmap, x1, y1);
    }
    ctx->skia_canvas->restore();

    return 1;
}

#else
// Random stars and random clip
int
sk_test_clip(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;

    shapes_t shape;
    shape_copy(&ctx->shape_defaults, &shape);
    ctx->skia_canvas->drawColor(SK_ColorBLACK);

    for (int i=0; i<ctx->size; i++) {
        double i = (double)rnd()/RAND_MAX * w;
        double j = (double)rnd()/RAND_MAX * h;
        ctx->skia_canvas->save();
        ctx->skia_canvas->translate(i, j);
        ctx->skia_canvas->scale(0.5,0.5);

        shape.x = 0;
        shape.y = 0;
        shape.radius = 40;
        shape.shape_type = CB_SHAPE_STAR;
        shape.fill_type = CB_FILL_SOLID;

        sk_set_fill_style(ctx, &shape);
        skiaDrawRandomizedShape(ctx, &shape);

        ctx->skia_canvas->clipPath(getCurrentSkiaPath(), SkRegion::kReplace_Op, true);
        double x1 = (double)rnd()/RAND_MAX * 80;
        double y1 = (double)rnd()/RAND_MAX * 80;
        ctx->skia_canvas->drawBitmap(bitmap, x1, y1);
        ctx->skia_canvas->restore();
    }

    return 1;
}
#endif
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
