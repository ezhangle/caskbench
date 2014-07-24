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
    SkColor colors[10];
    SkPoint center;

    center.fX = SkIntToScalar(20);
    center.fY = SkIntToScalar(20);

    for (int i=0; i<NUM_ELEM(colors); i++)
        colors[i] = skiaRandomColor();

    SkShader* shader = NULL;
    shader = SkGradientShader::CreateRadial(
        center, 20,
        colors, NULL, NUM_ELEM(colors),
        SkShader::kClamp_TileMode);

    // TODO: the HTML5 createRadialGradiet() routine
    // actually creates a conical gradient:  A gradient
    // is made between two circles C1 and C2.
    // x0=20, y0=20, r0=20, x1=50, y1=50, r1=70

    shapes_t shape;
    shape_copy(&ctx->shape_defaults, &shape);
    for (int i=0; i<ctx->size; i++) {
        double x1 = (double)rand()/RAND_MAX * w;
        double x2 = (double)rand()/RAND_MAX * w;
        double y1 = (double)rand()/RAND_MAX * h;
        double y2 = (double)rand()/RAND_MAX * h;

        double xx = MIN(x1, x2);
        double yy = MIN(x1, x2);
        double ww = abs(x2 - x1);
        double hh = abs(y2 - y1);

        // transform(shape.width/100, 0, 0, shape.height/100, 0, 0)
        shape.x = 0;
        shape.y = 0;
        shape.width = 100;
        shape.height = 100;
        shape.fill_type = CB_FILL_RADIAL_GRADIENT;

        skiaRandomizePaintColor(ctx);

        ctx->skia_paint->setStyle(SkPaint::kFill_Style);
        ctx->skia_paint->setShader(shader);

        skiaDrawRectangle(ctx, &shape);

        if (shader)
            shader->unref();
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
