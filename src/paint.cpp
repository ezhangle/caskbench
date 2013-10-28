#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
setup_paint(cairo_t *cr)
{
  return 1;
}

void
teardown_paint()
{
}

int
test_paint(cairo_t *cr)
{
  randomize_color (cr);
  cairo_paint (cr);
  return 1;
}
