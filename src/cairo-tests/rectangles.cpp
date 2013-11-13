#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
ca_setup_rectangles(caskbench_context_t *ctx)
{
  return 1;
}

void
ca_teardown_rectangles(void)
{
}

int
ca_test_rectangles(caskbench_context_t *ctx)
{
  int i, w, h, x, y;
  for (i=0; i<ctx->size; i++) {
    randomize_color (ctx->cr);
    w = (0.5*(double)ctx->canvas_width*rand())/RAND_MAX + 1;
    h = (0.5*(double)ctx->canvas_height*rand())/RAND_MAX + 1;
    x = (0.5*(double)ctx->canvas_width*rand())/RAND_MAX;
    y = (0.5*(double)ctx->canvas_height*rand())/RAND_MAX;

    cairo_rectangle(ctx->cr, x, y, w, h);
    cairo_set_line_width (ctx->cr, 1);
    cairo_stroke (ctx->cr);
  }
  return 1;
}
