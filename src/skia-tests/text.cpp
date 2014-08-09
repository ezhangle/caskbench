/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <unistd.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkGpuDevice.h>
#include <SkString.h>
#include <SkTemplates.h>
#include <SkTypeface.h>
#include <effects/SkGradientShader.h>
#include <SkDraw.h>
#include <effects/SkGradientShader.h>
#include <SkGraphics.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"

// This tests basic (non-glyph) text functionality

int
sk_setup_text(caskbench_context_t *ctx)
{
    SkTypeface *style = SkTypeface::CreateFromName("Serif", SkTypeface::kBold);

    ctx->skia_paint->setTypeface(style);
    ctx->skia_paint->setAntiAlias(false);
    ctx->skia_paint->setStyle(SkPaint::kFill_Style);
    ctx->skia_paint->setTextSize(20.0f);

    style->unref();

    return 1;
}

void
sk_teardown_text(void)
{
}

int
sk_test_text(caskbench_context_t *ctx)
{
    char text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t byteLength = strlen(text) * sizeof(char);
    double tw = SkScalarToDouble(ctx->skia_paint->measureText(text, byteLength));

    double w = (double)ctx->canvas_width;
    double h = (double)ctx->canvas_height;
    double off = abs (w - tw);

    ctx->skia_canvas->drawColor(SK_ColorBLACK);

    for (int i = 0; i < ctx->size; i++)
    {
        double x = drand48() * off;
        double y = drand48() * h;

        skiaRandomizePaintColor(ctx);

        ctx->skia_canvas->drawText(text, byteLength, SkDoubleToScalar(x),
                                   SkDoubleToScalar(y), *(ctx->skia_paint));
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
