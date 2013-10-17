#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

void
randomize_color(cairo_t *cr)
{
  double red, green, blue, alpha;

  red = (double)rand()/RAND_MAX;
  green = (double)rand()/RAND_MAX;
  blue = (double)rand()/RAND_MAX;
  alpha = (double)rand()/RAND_MAX;

  cairo_set_source_rgba (cr, red, green, blue, alpha);
}

int
main(int argc, char *argv[]) {
  int i, x, w, prev_w;
  cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 800, 80);
  cairo_t *cr = cairo_create (surface);

  srand(0xdeadbeef);

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

  cairo_destroy (cr);
  cairo_surface_write_to_png (surface, "hello.png");
  cairo_surface_destroy (surface);
  return 0;
}

