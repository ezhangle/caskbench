#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkBitmap.h>
#include <SkBitmapDevice.h>
#include <cairo.h>

#include "caskbench.h"

static SkBitmap bitmap;

int
sk_setup_image(caskbench_context_t *ctx)
{
    int i, x, y;
    bitmap.setConfig(SkBitmap::kARGB_8888_Config, 160, 40);
#ifdef USE_LEGACY_SKIA_SRA
    bitmap.allocPixels();
#else
    SkImageInfo info = SkImageInfo::Make(160, 40,
                                         kBGRA_8888_SkColorType,
                                         kPremul_SkAlphaType);
    bitmap.allocPixels(info);
#endif
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
sk_teardown_image(void)
{
}

int
sk_test_image(caskbench_context_t *ctx)
{
    int i, x, y;

    for (i=0; i<ctx->size; i++) {
        y = 50 * (int)(i/4);
        x = 200 * (i%4);
        ctx->skia_canvas->drawBitmap(bitmap, x, y);
    }

    return 1;
}
