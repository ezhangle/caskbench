// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include <SkCanvas.h>
#include <SkPaint.h>
#include <cairo.h>

#include "caskbench.h"
#include "SkGpuDevice.h"
#include "SkString.h"
#include "SkTemplates.h"
#include "SkTypeface.h"
static int max_dim;

	int
sk_setup_text(caskbench_context_t *ctx)
{
    max_dim = MIN(ctx->canvas_width, ctx->canvas_height)/2;
    return 1;
}

	void
sk_teardown_text(void)
{
}

	int
sk_test_text(caskbench_context_t *ctx)
{
    unsigned char red, green, blue, alpha;
    int i;
    ctx->skia_paint->setTextSize(SkIntToScalar(40));
    SkString text("TextRendering");
    for (i=0; i<ctx->size; i++) {
        red = int( 255 * (double)rand()/RAND_MAX );
	green = int( 255 * (double)rand()/RAND_MAX );
	blue = int( 255 * (double)rand()/RAND_MAX );
	alpha = int( 255 * (double)rand()/RAND_MAX );
	ctx->skia_paint->setLCDRenderText(true);
	ctx->skia_paint->setARGB(alpha, red, green, blue);
	ctx->skia_canvas->drawText(text.c_str(), text.size(), 150, 50, *ctx->skia_paint);
    }
    return 1;
}
