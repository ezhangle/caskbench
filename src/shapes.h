#ifndef __SHAPES_H_
#define __SHAPES_H_

#ifdef USE_SKIA
#  include <SkPath.h>
#  include <SkRect.h>
#endif

typedef struct shapes
{
    double center_x;
    double center_y;
    double radius;
    double width;
    double height;
    int numpoints;
    double (*points)[2];
#ifdef USE_SKIA
    SkPath path;
    SkRect rect;
#endif

    int shape_id;
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

enum Shapes
{
    Circle,
    Rectangle,
    Triangle,
    Star,
    RoundedRectangle
};

extern int star_points[11][2];

#endif
