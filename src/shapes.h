/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef __SHAPES_H_
#define __SHAPES_H_
#include <stdlib.h>
#include <stdint.h>

typedef enum {
    CB_FILL_RANDOM,
    CB_FILL_NONE,
    CB_FILL_SOLID,
    CB_FILL_LINEAR_GRADIENT,
    CB_FILL_RADIAL_GRADIENT,
    CB_FILL_IMAGE_PATTERN,
#if 0
    CB_FILL_HERRINGBONE_PATTERN,
#endif
    CB_FILL_END
} fill_type_t;

fill_type_t
convertToFillType(const char *fill_name);

typedef enum {
    CB_SHAPE_RANDOM,
    CB_SHAPE_CIRCLE,
    CB_SHAPE_RECTANGLE,
    CB_SHAPE_TRIANGLE,
    CB_SHAPE_STAR,
    CB_SHAPE_ROUNDED_RECTANGLE,
    CB_SHAPE_END
} shape_type_t;

shape_type_t
convertToShapeType(const char* shape_name);

typedef struct shapes
{
    double x;
    double y;
    double radius;
    double width;
    double height;
    double dx1;
    double dy1;
    double dx2;
    double dy2;

    shape_type_t shape_type;
    fill_type_t fill_type;
    uint32_t fill_color;
    uint32_t stroke_color;
    int animation;
    int stroke_width;

    int dash_style;
    int cap_style;
    int join_style;
} shapes_t;

extern int star_points[11][2];

void
shape_init(shapes_t *shape);

void
shape_copy(const shapes_t *shape_defaults, shapes_t *shape);

void
shape_randomize(shapes_t *shape);

shape_type_t
generate_random_shape();

void
generate_random_color(double &red, double &green, double &blue, double &alpha);

fill_type_t
generate_random_fill_type();

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
