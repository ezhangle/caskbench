#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
ca_setup_fill(cairo_t *cr)
{
  return 1;
}

void
ca_teardown_fill(void)
{
}

int
ca_test_fill(cairo_t *cr)
{
  int i;
  for (i=0; i<32; i++) {
    randomize_color (cr);
    cairo_arc (cr, 40*i, 40, 30, 0, 2*M_PI);
    cairo_fill_preserve (cr);
  }
  return 1;
}
