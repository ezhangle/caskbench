// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include "SkRect.h"
#include <cairo.h>
 
#include "caskbench.h"

int
sk_setup_roundrect(caskbench_context_t *ctx)
{
  ctx->paint->setARGB(255, 255, 255, 255);
  return 1;
}

int
sk_test_roundrect(caskbench_context_t *ctx)
{
  int i, x, y, w;
  SkRect rect;

  for (i=0; i<ctx->size; i++) {
    ctx->paint->setColor(rand());
    w = ((double)ctx->size*rand())/RAND_MAX + 1;
    x = ((double)ctx->canvas_width*rand())/RAND_MAX + 10;
    y = ((double)ctx->canvas_height*rand())/RAND_MAX + 10;
    rect.set(x, y, x+100, y+40);
    ctx->paint->setStrokeWidth(w);
    ctx->canvas->drawRoundRect(rect, 4.0, 4.0, *(ctx->paint));
  }

  return 1;
}

void
sk_teardown_roundrect(void)
{
}
