#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

static cairo_surface_t *image;

int
ca_setup_image(caskbench_context_t *ctx)
{
  image = cairo_image_surface_create_from_png ("../data/romedalen.png");
  cairo_set_source_surface (ctx->cr, image, 0, 0);
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

