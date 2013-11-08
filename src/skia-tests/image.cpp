// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include <err.h>

#include <SkBitmap.h>
#include <SkBitmapDevice.h>
#include <SkPaint.h>
#include <cairo.h>

#include "caskbench.h"

static SkBitmap bitmap;

int
sk_setup_image(caskbench_context_t *ctx)
{
  return 1;
}

int
sk_test_image(caskbench_context_t *ctx)
{
  int i;
  for (i=0; i<ctx->size; i++) {
    ctx->canvas->drawBitmap(bitmap, 0, 0);
  }

  return 1;
}

void
sk_teardown_image(void)
{
}

