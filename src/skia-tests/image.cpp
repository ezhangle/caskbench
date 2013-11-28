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
    int i;
    bitmap.setConfig(SkBitmap::kARGB_8888_Config, 100, 100);
    bitmap.allocPixels();
    SkBitmapDevice device(bitmap);
    SkCanvas canvas(&device);
    SkPaint paint;
    SkRect r;

    r.set(5, 5, 20, 20);

    for (i=0; i<16; i++) {
        paint.setARGB(255, 255/(i+1), 255, 16*i);
        r.offset(5, 5);
        canvas.drawRect(r, paint);
    }

    return 1;
}

void
sk_teardown_image(void)
{
}

int
sk_test_image(caskbench_context_t *ctx)
{
    int i;
    for (i=0; i<ctx->size; i++) {
        ctx->skia_canvas->drawBitmap(bitmap, 0, 0);
    }

    return 1;
}
