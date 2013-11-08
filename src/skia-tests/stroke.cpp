// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include "SkRect.h"
#include <cairo.h>
 
#include "caskbench.h"

static SkRect r;

int
sk_setup_stroke(caskbench_context_t *ctx)
{
  ctx->paint->setARGB(255, 255, 255, 255);
  r.set(10, 10, 20, 20);
  return 1;
}

int
sk_test_stroke(caskbench_context_t *ctx)
{
  int i, x, w, prev_w;
  x = 0;
  prev_w = 0;
  for (i=0; i<ctx->size; i++) {
    ctx->paint->setColor(rand());
    w = ((double)ctx->size*rand())/RAND_MAX + 1;
    x += 4 + (prev_w + w)/2.0;
    ctx->paint->setStrokeWidth(w);
    ctx->canvas->drawLine(x,10, x,70, *(ctx->paint));
    prev_w = w;
  }

  return 1;
}

void
sk_teardown_stroke(void)
{
}
