#ifndef __SHAPES_H_
#define __SHAPES_H_

#define MAX_SHAPES 5
#define MAX_FILL_TYPES 6

extern int star_points[11][2];
enum fillType
{
	none,
	solid,
	linearGradient,
	radialGradient,
	imagePattern,
	herringbonePattern
};

typedef struct shapes
{
    double center_x;
    double center_y;
    double radius;
    double width;
    double height;
    /* TODO Clip still uses points, modidy clip and remove numpoints and points */
    int numpoints;
    double (*points)[2];

    int shape_id;
    fillType fill_type;
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

#endif
