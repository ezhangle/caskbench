#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int
main(int argc, char *argv[]) {
  int i;
  int loops = 128*1024;
  cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 800, 80);
  cairo_t *cr = cairo_create (surface);
  cairo_pattern_t *mask;

  cairo_set_source_rgb (cr, 0, 0.5, 0);
  cairo_paint (cr);

  srand(0xdeadbeef);
  mask = cairo_pattern_create_rgba (0, 0, 0, 0.5);

  while (loops--) {
    cairo_mask (cr, mask);
  }

  cairo_destroy (cr);
  cairo_surface_write_to_png (surface, "mask.png");
  cairo_surface_destroy (surface);
  cairo_pattern_destroy (mask);
  return 0;
}

