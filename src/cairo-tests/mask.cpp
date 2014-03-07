#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

static cairo_pattern_t *mask;

int
ca_setup_mask(caskbench_context_t *ctx)
{
    // See https://git.gnome.org/browse/gtk+/tree/gtk/gtkcolorbutton.c?id=804c80097995bb0026544118a32b59e23ee2fbe3#n241
    static unsigned char data[16] = { 0xFF, 0x00, 0x00, 0xFF,
                                      0x00, 0xFF, 0xFF, 0x00,
                                      0x00, 0xFF, 0xFF, 0x00,
                                      0xFF, 0x00, 0x00, 0xFF, };
    static cairo_surface_t *stippled = NULL;

    if (stippled == NULL) {
        stippled = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_A8, 2, 2, 4);
    }
    mask = cairo_pattern_create_for_surface (stippled);
    cairo_pattern_set_extend (mask, CAIRO_EXTEND_REPEAT);
    cairo_pattern_set_filter (mask, CAIRO_FILTER_NEAREST);

    return 1;
}

void
ca_teardown_mask(void)
{
    cairo_pattern_destroy (mask);
}

int
ca_test_mask(caskbench_context_t *ctx)
{
    int i;
    cairo_t *cr = ctx->cairo_cr;

    for (i=0; i<ctx->size; i++) {
        // Apply mask on a circle
        double red, green, blue, alpha;
        red = (double)rand()/RAND_MAX;
        green = (double)rand()/RAND_MAX;
        blue = (double)rand()/RAND_MAX;
        alpha = (double)rand()/RAND_MAX;
        cairo_set_source_rgba (cr, red, green, blue, alpha);

        cairo_arc (cr, 40*i, 40, 30, 0, 2*M_PI);
        cairo_save (cr);
        cairo_clip (cr);
        cairo_mask (cr, mask);
        cairo_fill (cr);
        cairo_restore (cr);
    }
    return 1;
}
