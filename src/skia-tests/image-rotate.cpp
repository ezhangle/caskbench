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
sk_setup_image_rotate(caskbench_context_t *ctx)
{
    bitmap = skiaCreateSampleImage (ctx);
    return 1;
}

void
sk_teardown_image_rotate(void)
{
}

int
sk_test_image_rotate(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    int iw = bitmap.width();
    int ih = bitmap.height();
    int pw = w - iw;
    int ph = h - ih;

    for (int i=0; i<ctx->size; i++) {
        double x = (double)rand()/RAND_MAX * pw;
        double y = (double)rand()/RAND_MAX * ph;

        ctx->skia_canvas->save();
        ctx->skia_canvas->translate(w/2, h/2);
        ctx->skia_canvas->rotate(i / 50.0);  // TODO: Original rotates by counter/50
        ctx->skia_canvas->translate(-iw/2, -ih/2);
        ctx->skia_canvas->drawBitmap(bitmap, 0, 0);
        ctx->skia_canvas->restore();
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
