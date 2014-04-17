#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define WIDTH 800
#define HEIGHT 400

#define MAX_VELOCITY WIDTH / 2

typedef struct _kinetics
{
    double angle;
    double velocity;
    double x;
    double y;
    double color[4];

    double rotation;

    double x_radius;
    double y_radius;
    double x_offset;
    double y_offset;
    double width;
    double height;
    double line_width;

//Shadow, Drop ,Spread
    double shadow_color[4];
    double shadow_x_blur;
    double shadow_y_blur;

    double spread_color[4];
    double spread_line_width;
    double spread_x_blur;
    double spread_y_blur;

    double drop_color[4];
    double drop_x_blur;
    double drop_y_blur;
    double drop_x_offset;
    double drop_y_offset;
    double drop_line_width;
} kinetics_t;

void kinetics_init (kinetics_t *kinetics);
void kinetics_update (kinetics_t *kinetics, double delta);
void kinetics_destroy(kinetics_t *kinetics);
