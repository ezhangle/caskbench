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
#include "cairo-shapes.h"

int
ca_setup_paint(caskbench_context_t *ctx)
{
    return 1;
}

void
ca_teardown_paint()
{
}

int
ca_test_paint(caskbench_context_t *ctx)
{
    int i;

    for (i=0; i<ctx->size; i++) {
        cairoRandomizeColor(ctx);
        cairo_paint(ctx->cairo_cr);
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
