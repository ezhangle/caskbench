// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include <SkCanvas.h>
#include <SkPaint.h>
#include <cairo.h>

#include "caskbench.h"

int
sk_setup_fill(caskbench_context_t *ctx)
{
  return 1;
}

void
sk_teardown_fill(void)
{
}

int
sk_test_fill(caskbench_context_t *ctx)
{
  int i;
  for (i=0; i<ctx->size; i++) {
    ctx->skia_paint->setColor(rand());
    ctx->skia_canvas->drawCircle(40*i, 40, 30, *ctx->skia_paint);
  }

  return 1;
}
