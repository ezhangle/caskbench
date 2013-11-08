#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
ca_setup_fill(caskbench_context_t *ctx)
{
  return 1;
}

void
ca_teardown_fill(void)
{
}

int
ca_test_fill(caskbench_context_t *ctx)
{
  int i;
  for (i=0; i<ctx->size; i++) {
    randomize_color (ctx->cr);
    cairo_arc (ctx->cr, 40*i, 40, 30, 0, 2*M_PI);
    cairo_fill (ctx->cr);
  }
  return 1;
}
