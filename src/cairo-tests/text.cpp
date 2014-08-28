/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <string.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"

// This tests basic (non-glyph) text functionality

int
ca_setup_text(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;
    cairo_select_font_face(cr, "Serif",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(cr, 20);
    cairo_set_antialias(cr,CAIRO_ANTIALIAS_NONE);
    return 1;
}

void
ca_teardown_text(void)
{
}

int
ca_test_text(caskbench_context_t *ctx)
{
    char text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    cairo_t *cr = ctx->cairo_cr;
    size_t byteLength = strlen(text) * sizeof(char);
    cairo_text_extents_t  text_metrics;
    cairo_text_extents (cr, text, &text_metrics);
    double tw = text_metrics.width + 3; // Adding 3 to make tw value same as skia text.
    double w = (double) ctx->canvas_width;
    double h = (double) ctx->canvas_height;
    double off = abs (w - tw);

    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_rectangle (cr, 0, 0, ctx->canvas_width ,ctx->canvas_height);
    cairo_fill (cr);

    for (int i = 0; i < ctx->size; i++)
    {
        double x = drnd48() * off;
        double y = drnd48() * h;

        cairoRandomizeColor(ctx);
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, text);
    }
    return 1;
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
