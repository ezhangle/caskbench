/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkBitmap.h>
#include <SkBitmapDevice.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"

static SkBitmap bitmap;

int
sk_setup_image_scale(caskbench_context_t *ctx)
{
    int i, x, y;
    bitmap.setConfig(SkBitmap::kARGB_8888_Config, 160, 40);
    SkImageInfo info = SkImageInfo::Make(160, 40,
                                         kBGRA_8888_SkColorType,
                                         kPremul_SkAlphaType);
    bitmap.allocPixels(info);
    SkBitmapDevice device(bitmap);
    SkCanvas canvas(&device);
    SkPaint paint;
    SkRect r;

    canvas.clear(0);

    x = 5;
    for (i=0; i<16; i++) {
        paint.setARGB(255, 255/(i+1), 255, 16*i);
        x += 10;
        y = (i%2)*10;
        r.set(x, y, x+10, y+10);

        canvas.drawRect(r, paint);
    }
    canvas.flush();

    return 1;
}

void
sk_teardown_image_scale(void)
{
}

int
sk_test_image_scale(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    SkRect r;

    for (int i=0; i<ctx->size; i++) {
        double x1 = (double)rand()/RAND_MAX * w;
        double x2 = (double)rand()/RAND_MAX * w;
        double y1 = (double)rand()/RAND_MAX * h;
        double y2 = (double)rand()/RAND_MAX * h;
        double x = MIN(x1, x2);
        double y = MIN(y1, y2);
        r.set(x, y, x + fabs(x2 - x1), y + fabs(y2 - y1));
        ctx->skia_canvas->drawBitmapRect(bitmap, r, ctx->skia_paint);
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
