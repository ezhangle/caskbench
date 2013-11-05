#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
ca_setup_paint(cairo_t *cr)
{
  return 1;
}

void
ca_teardown_paint()
{
}

int
ca_test_paint(cairo_t *cr)
{
  randomize_color (cr);
  cairo_paint (cr);
  return 1;
}
