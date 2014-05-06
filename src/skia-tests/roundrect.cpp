#include <config.h>

#include <math.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>

#include "caskbench.h"
#include "skia-shapes.h"

int
sk_setup_roundrect(caskbench_context_t *ctx)
{
    ctx->skia_paint->setAntiAlias(true);
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    return 1;
}

void
sk_teardown_roundrect(void)
{
}

int
sk_test_roundrect(caskbench_context_t *ctx)
{
    int i;
    double line_width, x, y;
    SkRect rect;

    for (i=0; i<ctx->size; i++) {
        skiaRandomizeColor(ctx);

        x = trunc( (((double)ctx->canvas_width-20)*rand())/RAND_MAX ) + 10;
        y = trunc( (((double)ctx->canvas_height-20)*rand())/RAND_MAX ) + 10;
        rect.set(x, y, x+100, y+40);

        line_width = trunc( ((double)ctx->size*rand())/RAND_MAX ) + 1;
        ctx->skia_paint->setStrokeWidth(line_width);

        ctx->skia_canvas->drawRoundRect(rect, 4.0, 4.0, *(ctx->skia_paint));
    }

    return 1;
}
