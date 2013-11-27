#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>
#include <cairo.h>

#include "caskbench.h"

static SkRect r;
static int line_length;

int
sk_setup_stroke(caskbench_context_t *ctx)
{
  ctx->skia_paint->setARGB(255, 255, 255, 255);
  r.set(10, 10, 20, 20);
  line_length = 60;
  return 1;
}

void
sk_teardown_stroke(void)
{
}

int
sk_test_stroke(caskbench_context_t *ctx)
{
  int i, x, w, prev_w;
  x = 0;
  prev_w = 0;
  for (i=0; i<ctx->size; i++) {
    ctx->skia_paint->setColor(rand());
    w = ((double)ctx->size*rand())/RAND_MAX + 1;
    x += 4 + (prev_w + w)/2.0;
    ctx->skia_paint->setStrokeWidth(w);
    ctx->skia_canvas->drawLine(x,10, x,10+line_length, *(ctx->skia_paint));
    prev_w = w;
  }

  return 1;
}
