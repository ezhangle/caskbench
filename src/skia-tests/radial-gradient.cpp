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
#include <SkGradientShader.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"

int
sk_setup_radial_gradient(caskbench_context_t *ctx)
{
    return 1;
}

void
sk_teardown_radial_gradient(void)
{
}

int
sk_test_radial_gradient(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    int stops = 10;

    SkPoint pts[2];
    pts[0].iset(20, 20);
    pts[1].iset(50, 50);
    SkScalar radii[2];
    radii[0] = 20.0f;
    radii[1] = 70.0f;
    SkColor colors[10];
    SkScalar pos[10];

    for (int i = 0; i < stops; i++) {
        pos[i] = i / SkIntToScalar(stops - 1);
        colors[i] = skiaRandomColor();
    }

    SkShader *shader = SkGradientShader::CreateTwoPointRadial(pts[0], radii[0],
                                          pts[1], radii[1], colors, pos, stops,
                                          SkShader::kClamp_TileMode);

    shapes_t shape;
    shape_copy(&ctx->shape_defaults, &shape);
    for (int i=0; i<ctx->size; i++) {
        double x1 = (double)rnd()/RAND_MAX * w;
        double x2 = (double)rnd()/RAND_MAX * w;
        double y1 = (double)rnd()/RAND_MAX * h;
        double y2 = (double)rnd()/RAND_MAX * h;

        double xx = MIN(x1, x2);
        double yy = MIN(x1, x2);
        double ww = abs(x2 - x1);
        double hh = abs(y2 - y1);

        ctx->skia_canvas->save();
        ctx->skia_canvas->translate(SkDoubleToScalar(xx), SkDoubleToScalar(yy));
        ctx->skia_canvas->scale(SkDoubleToScalar(ww/100), SkDoubleToScalar(hh/100));

        // transform(shape.width/100, 0, 0, shape.height/100, 0, 0)
        shape.x = 0;
        shape.y = 0;
        shape.width = 100;
        shape.height = 100;
        shape.fill_type = CB_FILL_RADIAL_GRADIENT;


        ctx->skia_paint->setStyle(SkPaint::kFill_Style);
        ctx->skia_paint->setShader(shader);

        skiaDrawRectangle(ctx, &shape);
        ctx->skia_canvas->restore();

    }
    if (shader)
        shader->unref();

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
