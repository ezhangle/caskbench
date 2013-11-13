#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include "SkRect.h"
#include <cairo.h>
 
#include "caskbench.h"

int
sk_setup_rectangles(caskbench_context_t *ctx)
{
  ctx->paint->setARGB(255, 255, 255, 255);
  return 1;
}

int
sk_test_rectangles(caskbench_context_t *ctx)
{
  int i, x, y, w, h;
  SkRect rect;

  for (i=0; i<ctx->size; i++) {
    ctx->paint->setColor(rand());
    w = (0.5*(double)ctx->canvas_width*rand())/RAND_MAX + 1;
    h = (0.5*(double)ctx->canvas_height*rand())/RAND_MAX + 1;
    x = (0.5*(double)ctx->canvas_width*rand())/RAND_MAX;
    y = (0.5*(double)ctx->canvas_height*rand())/RAND_MAX;
    rect.set(x, y, x+w, y+h);
    ctx->paint->setStrokeWidth(1);
    ctx->canvas->drawRect(rect, *(ctx->paint));
  }

  return 1;
}

void
sk_teardown_rectangles(void)
{
}
