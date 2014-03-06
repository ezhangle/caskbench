#include <math.h>
#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>
#include <cairo.h>

#include "caskbench.h"

int
sk_setup_rectangles(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(false);
    ctx->skia_paint->setStrokeWidth(1);
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    return 1;
}

void
sk_teardown_rectangles(void)
{
}

int
sk_test_rectangles(caskbench_context_t *ctx)
{
    unsigned char red, green, blue, alpha;
    int i, x, y, w, h;
    SkRect rect;

    for (i=0; i<ctx->size; i++) {
        red = int( 255 * (double)rand()/RAND_MAX );
        green = int( 255 * (double)rand()/RAND_MAX );
        blue = int( 255 * (double)rand()/RAND_MAX );
        alpha = int( 255 * (double)rand()/RAND_MAX );
        ctx->skia_paint->setARGB(alpha, red, green, blue);

        w = trunc( (0.5*(double)ctx->canvas_width*rand())/RAND_MAX ) + 1;
        h = trunc( (0.5*(double)ctx->canvas_height*rand())/RAND_MAX ) + 1;
        x = trunc( (0.5*(double)ctx->canvas_width*rand())/RAND_MAX );
        y = trunc( (0.5*(double)ctx->canvas_height*rand())/RAND_MAX );
        rect.set(x, y, x+w, y+h);

        ctx->skia_canvas->drawRect(rect, *(ctx->skia_paint));
    }

    return 1;
}
