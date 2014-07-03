/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef __CAIRO_SHAPES_H_
#define __CAIRO_SHAPES_H_

#include "forward.h"
#include "shapes.h"
#include "caskbench.h"

void cairoRandomizeColor(caskbench_context_t *cr);

void ca_set_fill_style(caskbench_context_t *ctx, const shapes_t *shape);

void cairoDrawCircle(caskbench_context_t *ctx, shapes_t *args);

void cairoDrawRectangle(caskbench_context_t *ctx, shapes_t *args);

void cairoDrawTriangle(caskbench_context_t *ctx, shapes_t *args);

void cairoDrawStar(caskbench_context_t *ctx, shapes_t *args);

void cairoDrawRoundedRectangle (caskbench_context_t *ctx, shapes_t *args);

void cairoDrawLine(caskbench_context_t *ctx, shapes_t *args);

void cairoDrawCubicCurve(caskbench_context_t *ctx, shapes_t *args);

void cairoDrawQuadraticCurve(caskbench_context_t *ctx, shapes_t *args);

cairo_surface_t *cairoCreateSampleImage (caskbench_context_t *ctx);

cairo_surface_t *cairoCacheImageSurface(caskbench_context_t *ctx, cairo_surface_t *image);

extern void (*cairoShapes[CB_SHAPE_END-1])(caskbench_context_t *ctx , shapes_t *args);

void cairoDrawRandomizedShape(caskbench_context_t *ctx, shapes_t *shape);

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
