#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

static int max_dim;

int
ca_setup_text(caskbench_context_t *ctx)
{
    max_dim = MIN(ctx->canvas_width, ctx->canvas_height)/2;
    return 1;
}

void
ca_teardown_text(void)
{
}

int
ca_test_text(caskbench_context_t *ctx)
{
    int i, x, y, r;
    cairo_t *cr = ctx->cairo_cr;
    cairo_set_font_size (cr, 40);

    for (i=0; i<ctx->size; i++) {
        cairo_text_extents_t extents;
        cairo_font_options_t *font_options;
        float offsetY,offsetX;
        int align;
        unsigned int flags;

	randomize_color(cr);
        cairo_move_to (cr,  150, 50);
        cairo_show_text (cr, "TextRendering");
    }

    return 1;
}
