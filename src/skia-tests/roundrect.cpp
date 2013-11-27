#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>
#include <cairo.h>

#include "caskbench.h"

int
sk_setup_roundrect(caskbench_context_t *ctx)
{
  ctx->skia_paint->setARGB(255, 255, 255, 255);
  return 1;
}

void
sk_teardown_roundrect(void)
{
}

int
sk_test_roundrect(caskbench_context_t *ctx)
{
  int i, x, y, w;
  SkRect rect;

  for (i=0; i<ctx->size; i++) {
    ctx->skia_paint->setColor(rand());
    w = ((double)ctx->size*rand())/RAND_MAX + 1;
    x = ((double)ctx->canvas_width*rand())/RAND_MAX + 10;
    y = ((double)ctx->canvas_height*rand())/RAND_MAX + 10;
    rect.set(x, y, x+100, y+40);
    ctx->skia_paint->setStrokeWidth(w);
    ctx->skia_canvas->drawRoundRect(rect, 4.0, 4.0, *(ctx->skia_paint));
  }

  return 1;
}
