#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

static cairo_surface_t *image;

int
ca_setup_image(caskbench_context_t *ctx)
{
    int i, x, y;
    cairo_t *cr_image;

    image = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 160, 40);
    cr_image = cairo_create (image);
    cairo_set_source_surface (cr_image, image, 0, 0);

    x = 5;
    for (i=0; i<16; i++) {
        x += 10;
        y = (i%2)*10;

        cairo_set_source_rgba (cr_image, 1.0/(i+1), 1.0, 16.0*i/255.0, 1.0);
        cairo_rectangle (cr_image, x, y, 10, 10);
        cairo_fill (cr_image);
    }

    cairo_destroy (cr_image);

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
    int i, x, y;
    cairo_t *cr = ctx->cairo_cr;

    for (i=0; i<ctx->size; i++) {
        y = 50 * (int)(i/4);
        x = 200 * (i%4);
        cairo_set_source_surface (cr, image, x, y);
        cairo_paint (cr);
    }
    return 1;
}
