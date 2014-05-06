#ifndef __SKIA_SHAPES_H_
#define __SKIA_SHAPES_H_

#include <cairo.h>
#include <math.h>

#include "shapes.h"
#include "caskbench.h"

void skiaRandomizeColor(caskbench_context_t *ctx);

void skiaDrawCircle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawRectangle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawTriangle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawStar(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawRoundedRectangle(caskbench_context_t *ctx, shapes_t *args);

extern void (*skiaShapes[5])(caskbench_context_t *ctx , shapes_t *args);

#endif
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
