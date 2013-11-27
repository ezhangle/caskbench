#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>

#include "caskbench.h"

static int element_spacing;
static int num_x_elements;
static int num_y_elements;

int
ca_setup_multishape(caskbench_context_t *ctx)
{
  if (ctx->size < 0)
    return 0;

  element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
  num_x_elements = ctx->canvas_width / element_spacing;
  num_y_elements = ctx->canvas_height / element_spacing;

  return 1;
}

void
ca_teardown_multishape(void)
{
}

int
ca_test_multishape(caskbench_context_t *ctx)
{
  int i, j, x, y, r;
  cairo_t *cr = ctx->cairo_cr;

  r = 0.9 * element_spacing / 2;
  for (j=0; j<num_y_elements; j++) {
    y = j * element_spacing;
    for (i=0; i<num_x_elements; i++) {
      x = i * element_spacing;
      randomize_color (cr);
      cairo_arc (cr, x, y, r, 0, 2*M_PI);
      cairo_fill (cr);
    }
  }
  return 1;
}
