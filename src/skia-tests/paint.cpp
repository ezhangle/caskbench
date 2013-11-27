#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>
#include <cairo.h>

#include "caskbench.h"

static SkRect r;

int
sk_setup_paint(caskbench_context_t *ctx)
{
  return 1;
}

void
sk_teardown_paint(void)
{
}

int
sk_test_paint(caskbench_context_t *ctx)
{
  int i;
  for (i=0; i<ctx->size; i++) {
    ctx->skia_canvas->drawColor(rand());
  }

  return 1;
}
