#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

static int line_length;

int
ca_setup_stroke(caskbench_context_t *ctx)
{
  line_length = 60;
  return 1;
}

void
ca_teardown_stroke(void)
{
}

int
ca_test_stroke(caskbench_context_t *ctx)
{
  int i, x, w, prev_w;
  x = 0;
  prev_w = 0;
  for (i=0; i<ctx->size; i++) {
    randomize_color (ctx->cr);
    w = ((double)ctx->size*rand())/RAND_MAX + 1;
    x += 4 + (prev_w + w)/2.0;
    cairo_move_to (ctx->cr, x, 10);
    cairo_line_to (ctx->cr, x, 10 + line_length);
    cairo_set_line_width (ctx->cr, w);
    cairo_stroke (ctx->cr);
    prev_w = w;
  }
  return 1;
}
