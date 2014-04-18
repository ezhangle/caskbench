// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/
#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <cairo.h>

#include "caskbench.h"

static int max_dim;

int
sk_setup_bubbles(caskbench_context_t *ctx)
{
    max_dim = MIN(ctx->canvas_width, ctx->canvas_height)/2;
    return 1;
}

void
sk_teardown_bubbles(void)
{
}

int
sk_test_bubbles(caskbench_context_t *ctx)
{
    unsigned char red, green, blue, alpha;
    int i, x, y, r;
    for (i=0; i<ctx->size; i++) {
        red = int( 255 * (double)rand()/RAND_MAX );
        green = int( 255 * (double)rand()/RAND_MAX );
        blue = int( 255 * (double)rand()/RAND_MAX );
        alpha = int( 255 * (double)rand()/RAND_MAX );
        ctx->skia_paint->setARGB(alpha, red, green, blue);

        r = ((double)max_dim*rand())/RAND_MAX + 1;
        x = (0.5*(double)ctx->canvas_width*rand())/RAND_MAX;
        y = (0.5*(double)ctx->canvas_height*rand())/RAND_MAX;
        ctx->skia_canvas->drawCircle(x, y, r, *ctx->skia_paint);
    }

    return 1;
}
