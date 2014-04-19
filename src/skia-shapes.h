#ifndef __SKIA_SHAPES_H_
#define __SKIA_SHAPES_H_

#include <SkCanvas.h>
#include <SkPaint.h>
#include <cairo.h>
#include <math.h>

#include "shapes.h"
#include "caskbench.h"

void skiaDrawCircle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawRectangle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawTriangle(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawStar(caskbench_context_t *ctx, shapes_t *args);

void skiaDrawRoundedRectangle(caskbench_context_t *ctx, shapes_t *args);

extern void (*skiaShapes[5])(caskbench_context_t *ctx , shapes_t *args);

#endif
