#include <SkCanvas.h>
#include <SkPaint.h>
#include <effects/SkStippleMaskFilter.h>
#include <cairo.h>

#include "caskbench.h"

int
sk_setup_mask(caskbench_context_t *ctx)
{
    SkMaskFilter *mask = new SkStippleMaskFilter();
    ctx->skia_paint->setMaskFilter(mask);
    return 1;
}

void
sk_teardown_mask(void)
{
}

int
sk_test_mask(caskbench_context_t *ctx)
{
    unsigned char red, green, blue, alpha;
    int i;

    for (i=0; i<ctx->size; i++) {
        red = int( 255 * (double)rand()/RAND_MAX );
        green = int( 255 * (double)rand()/RAND_MAX );
        blue = int( 255 * (double)rand()/RAND_MAX );
        alpha = int( 255 * (double)rand()/RAND_MAX );
        ctx->skia_paint->setARGB(alpha, red, green, blue);

        // Apply mask on a circle
        ctx->skia_canvas->drawCircle(40*i, 40, 30, *ctx->skia_paint);
    }

    return 1;
}
