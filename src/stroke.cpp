#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
setup_stroke(cairo_t *cr)
{
  return 1;
}

void
teardown_stroke(void)
{
}

int
test_stroke(cairo_t *cr)
{
  int i, x, w, prev_w;
  x = 0;
  prev_w = 0;
  for (i=0; i<32; i++) {
    randomize_color (cr);
    w = (32.0*rand())/RAND_MAX + 1;
    x += 4 + (prev_w + w)/2.0;
    cairo_move_to (cr, x, 10);
    cairo_line_to (cr, x, 70);
    cairo_set_line_width (cr, w);
    cairo_stroke (cr);
    prev_w = w;
  }
  return 1;
}
