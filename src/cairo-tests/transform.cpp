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

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"

int
ca_setup_transform(caskbench_context_t *ctx)
{
    cairo_set_antialias (ctx->cairo_cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width (ctx->cairo_cr, 1);
    return 1;
}

void
ca_teardown_transform(void)
{
}

int
ca_test_transform(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;
    double rotation_delta(0.02*180/M_PI);
    cairo_t *cr = ctx->cairo_cr;

    cairo_save (cr);
    for (int nn=0; nn<ctx->size; nn++) {
        for (int i=0; i<w; i+=50) {
            cairo_new_path (cr);
            cairo_move_to (cr, i, 0);
            cairo_line_to (cr, i, h);
            cairoRandomizeColor(ctx);
            cairo_stroke (cr);
        }
        for (int j=0; j<h; j+=50) {
            cairo_move_to(cr, 0, j);
            cairo_line_to(cr, w, j);
            cairoRandomizeColor(ctx);
            cairo_stroke (cr);
        }
        cairo_translate(cr, w/2.0, h/2.0);
        cairo_rotate(cr,  (1/57.29)*rotation_delta);
        cairo_scale(cr, 1.004, 0.996);
        cairo_translate(cr, -w/2.0, -h/2.0);
    }
    cairo_restore (cr);

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
