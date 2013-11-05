#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
ca_setup_fill(cairo_t *cr)
{
  cairo_arc (cr, 40, 40, 30, 0, 2*M_PI);
  return 1;
}

void
ca_teardown_fill(void)
{
}

int
ca_test_fill(cairo_t *cr)
{
  randomize_color (cr);
  cairo_fill_preserve (cr);
  return 1;
}
