/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef __SHAPES_H_
#define __SHAPES_H_

typedef enum {
    CB_FILL_NONE,
    CB_FILL_SOLID,
    CB_FILL_LINEAR_GRADIENT,
    CB_FILL_RADIAL_GRADIENT,
    CB_FILL_IMAGE_PATTERN,
    CB_FILL_HERRINGBONE_PATTERN
} fill_type_t;

typedef enum {
    CB_SHAPE_CIRCLE,
    CB_SHAPE_RECTANGLE,
    CB_SHAPE_TRIANGLE,
    CB_SHAPE_STAR,
    CB_SHAPE_ROUNDED_RECTANGLE
} shape_type_t;

typedef struct shapes
{
    double x;
    double y;
    double radius;
    double width;
    double height;

    int shape_id;
    char* shape_name;
    char* fill_type;
    double red;
    double green;
    double blue;
    double alpha;
    int animation;
    char *image_path;
    int stroke_width;
    int multi_shapes;

    double stroke_red;
    double stroke_green;
    double stroke_blue;

    int dash_style;
    int cap_style;
    int join_style;
} shapes_t;

extern int star_points[11][2];

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
