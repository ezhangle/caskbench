#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>
#include <cairo.h>

#include "caskbench.h"

static SkRect r;
static int line_length;

int
sk_setup_stroke(caskbench_context_t *ctx)
{
    r.set(10, 10, 20, 20);
    line_length = 60;
    return 1;
}

void
sk_teardown_stroke(void)
{
}

int
sk_test_stroke(caskbench_context_t *ctx)
{
    unsigned char red, green, blue, alpha;
    int i, x, w, prev_w;
    x = 0;
    prev_w = 0;
    for (i=0; i<ctx->size; i++) {
        red = int( 255 * (double)rand()/RAND_MAX );
        green = int( 255 * (double)rand()/RAND_MAX );
        blue = int( 255 * (double)rand()/RAND_MAX );
        alpha = int( 255 * (double)rand()/RAND_MAX );
        ctx->skia_paint->setARGB(alpha, red, green, blue);

        w = ((double)ctx->size*rand())/RAND_MAX + 1;
        x += 4 + (prev_w + w)/2.0;
        ctx->skia_paint->setStrokeWidth(w);
        ctx->skia_canvas->drawLine(x,10, x,10+line_length, *(ctx->skia_paint));
        prev_w = w;
    }

    return 1;
}
