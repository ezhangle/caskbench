/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/
#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"

static int max_dim;

int
sk_setup_bubbles(caskbench_context_t *ctx)
{
    max_dim = MIN(ctx->canvas_width, ctx->canvas_height)/2;
    return 1;
}

void
sk_teardown_bubbles(void)
{
}

int
sk_test_bubbles(caskbench_context_t *ctx)
{
    int i, x, y, r;
    for (i=0; i<ctx->size; i++) {
        skiaRandomizePaintColor(ctx);

        r = ((double)max_dim*rand())/RAND_MAX + 1;
        x = (0.5*(double)ctx->canvas_width*rand())/RAND_MAX;
        y = (0.5*(double)ctx->canvas_height*rand())/RAND_MAX;
        ctx->skia_canvas->drawCircle(x, y, r, *ctx->skia_paint);
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
