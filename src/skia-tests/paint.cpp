#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>

#include "caskbench.h"

static SkRect r;

int
sk_setup_paint(caskbench_context_t *ctx)
{
    return 1;
}

void
sk_teardown_paint(void)
{
}

int
sk_test_paint(caskbench_context_t *ctx)
{
    unsigned char red, green, blue, alpha;
    int i;
    for (i=0; i<ctx->size; i++) {
        red = int( 255 * (double)rand()/RAND_MAX );
        green = int( 255 * (double)rand()/RAND_MAX );
        blue = int( 255 * (double)rand()/RAND_MAX );
        alpha = int( 255 * (double)rand()/RAND_MAX );

        ctx->skia_paint->setARGB(alpha, red, green, blue);
        ctx->skia_canvas->drawPaint(*ctx->skia_paint);
    }

    return 1;
}
