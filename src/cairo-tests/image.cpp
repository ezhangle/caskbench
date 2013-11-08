#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

static cairo_surface_t *image;

int
ca_setup_image(caskbench_context_t *ctx)
{
  cairo_t *cr_image;
  int i, x, y;
  image = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 100, 100);
  cr_image = cairo_create(image);
  cairo_set_source_surface (cr_image, image, 0, 0);


  x = y = 5;
  for (i=0; i<16; i++) {
    x += 5;
    y += 5;
    cairo_set_source_rgba (cr_image, 1.0, 1.0/(i+1), 1.0, 16*i/255.0);
    cairo_rectangle(cr_image, x, y, 20, 20);
  }

  return 1;
}

void
ca_teardown_image(void)
{
  cairo_surface_destroy (image);
}

int
ca_test_image(caskbench_context_t* ctx)
{
  int i;
  for (i=0; i<ctx->size; i++)
    cairo_paint (ctx->cr);
  return 1;
}  

