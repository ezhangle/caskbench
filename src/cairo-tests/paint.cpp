#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
ca_setup_paint(caskbench_context_t *ctx)
{
  return 1;
}

void
ca_teardown_paint()
{
}

int
ca_test_paint(caskbench_context_t *ctx)
{
  int i;
  for (i=0; i<ctx->size; i++) {
    randomize_color (ctx->cr);
    cairo_paint (ctx->cr);
  }
  return 1;
}
