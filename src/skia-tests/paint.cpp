// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include "SkRect.h"
#include <cairo.h>
 
#include "caskbench.h"

static SkRect r;

int
sk_setup_paint(caskbench_context_t *ctx)
{
  return 1;
}

int
sk_test_paint(caskbench_context_t *ctx)
{
  int i;
  for (i=0; i<ctx->size; i++) {
    ctx->canvas->drawColor(rand());
  }

  return 1;
}

void
sk_teardown_paint(void)
{
}
