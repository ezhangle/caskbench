#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

static cairo_pattern_t *mask;

int
ca_setup_mask(caskbench_context_t *ctx)
{
  mask = cairo_pattern_create_rgba (0, 0, 0, 0.5);
  return 1;
}

void
ca_teardown_mask(void)
{
  cairo_pattern_destroy (mask);
}

int
ca_test_mask(caskbench_context_t *ctx)
{
  int i;
  for (i=0; i<ctx->size; i++)
    cairo_mask (ctx->cr, mask);
  return 1;
}
