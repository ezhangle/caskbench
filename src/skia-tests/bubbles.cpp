// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include <cairo.h>
 
#include "caskbench.h"

static int max_dim;

int
sk_setup_bubbles(caskbench_context_t *ctx)
{
  max_dim = MIN(ctx->canvas_width, ctx->canvas_height)/2;
  return 1;
}

int
sk_test_bubbles(caskbench_context_t *ctx)
{
  int i, x, y, r;
  for (i=0; i<ctx->size; i++) {
    r = ((double)max_dim*rand())/RAND_MAX + 1;
    x = (0.5*(double)ctx->canvas_width*rand())/RAND_MAX;
    y = (0.5*(double)ctx->canvas_height*rand())/RAND_MAX;
    ctx->paint->setColor(rand());
    ctx->canvas->drawCircle(x, y, r, *ctx->paint);
  }

  return 1;
}

void
sk_teardown_bubbles(void)
{
}

