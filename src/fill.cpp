#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

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
  int i;
  int iterations_requested = 16*1024;
  int iterations = iterations_requested;
  cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 800, 80);
  cairo_t *cr = cairo_create (surface);
  int status = 0;

  srand(0xdeadbeef);
  cairo_arc (cr, 40, 40, 30, 0, 2*M_PI);

  while (iterations--) {
    try {
      randomize_color (cr);
      cairo_fill_preserve (cr);
      status = 1;
    } catch (...) {
      printf("Exception encountered\n");
      status = 3;
      goto FINAL;
    }
  }

  cairo_surface_write_to_png (surface, "fill.png");

 FINAL:
  printf("   {\n");
  printf("       \"case\": \"fill\",\n");
  printf("       \"iterations\": %d,\n", iterations_requested - iterations);
  printf("       \"minimum run time (s)\": %f,\n", 40.123);
  printf("       \"median run time (s)\": %f,\n", 42.123);
  printf("       \"standard deviation (%%)\": %f,\n", 42.123);
  printf("       \"status\": \"%d\"\n", status);
  printf("   }");
  printf("\n");

  cairo_destroy (cr);
  cairo_surface_destroy (surface);
  return 0;
}

