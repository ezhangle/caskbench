#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

static int line_length;

int
ca_setup_roundrect(caskbench_context_t *ctx)
{
  line_length = 60;
  return 1;
}

void
ca_teardown_roundrect(void)
{
}

static void
rounded_rectangle (cairo_t *cr,
                   double x, double y, double w, double h,
                   double radius)
{
  cairo_move_to (cr, x+radius, y);
  cairo_arc (cr, x+w-radius, y+radius,   radius, M_PI + M_PI / 2, M_PI * 2        );
  cairo_arc (cr, x+w-radius, y+h-radius, radius, 0,               M_PI / 2        );
  cairo_arc (cr, x+radius,   y+h-radius, radius, M_PI/2,          M_PI            );
  cairo_arc (cr, x+radius,   y+radius,   radius, M_PI,            270 * M_PI / 180);
}


int
ca_test_roundrect(caskbench_context_t *ctx)
{
  int i, w, x, y;
  for (i=0; i<ctx->size; i++) {
    randomize_color (ctx->cr);
    x = 10 + ((double)800*rand())/RAND_MAX;
    y = 10 + ((double)300*rand())/RAND_MAX;;
    w = ((double)ctx->size*rand())/RAND_MAX + 1;

    rounded_rectangle (ctx->cr, x, y, 100, 40, 4);
    cairo_set_line_width (ctx->cr, w);
    cairo_stroke (ctx->cr);
  }
  return 1;
}
