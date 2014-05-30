/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"

static SkRect r;

int
sk_setup_paint(caskbench_context_t *ctx)
{
    return 1;
}

void
sk_teardown_paint(void)
{
}

int
sk_test_paint(caskbench_context_t *ctx)
{
    int i;
    for (i=0; i<ctx->size; i++) {
        skiaRandomizePaintColor(ctx);
        ctx->skia_canvas->drawPaint(*ctx->skia_paint);
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
