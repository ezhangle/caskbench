/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef __SKIA_SHAPES_H_
#define __SKIA_SHAPES_H_

#include <SkColor.h>

#include "forward.h"
#include "shapes.h"

SkColor skiaRandomColor();

void skiaRandomizePaintColor(caskbench_context_t *ctx);

void sk_set_fill_style(caskbench_context_t *ctx, const shapes_t *shape);

SkShader *skiaCreateLinearGradientShader(int y1, int y2);

SkShader *skiaCreateRadialGradientShader(const shapes_t *shape);

SkShader *skiaCreateBitmapShader(const char *image_path);
void skiaDrawLine(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawQuadraticCurve(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawCubicCurve(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawCircle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawRectangle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawTriangle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawStar(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawRoundedRectangle(caskbench_context_t *ctx, shapes_t *args);

extern void (*skiaShapes[CB_SHAPE_END-1])(caskbench_context_t *ctx , shapes_t *args);

void skiaDrawRandomizedShape(caskbench_context_t *ctx, shapes_t *shape);

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
