/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <err.h>
#include <string.h>

#include "shapes.h"

const char *gFillTypes[] = {
    "random",
    "none",
    "solid",
    "linear-gradient",
    "radial-gradient",
    "image-pattern",
    "herringbone-pattern",
    NULL
};

fill_type_t
convertToFillType(const char *fill_name)
{
    int i =0;
    if (fill_name == NULL)
        return (fill_type_t) 0;
    while (gFillTypes[i] != NULL) {
        if (strcmp(gFillTypes[i], fill_name) == 0)
            return (fill_type_t) i ;
        i++;
    }
    errx(1, "Unknown fill type '%s' specified", fill_name);
}

const char *gShapes[] = {
    "random",
    "circle",
    "rectangle",
    "triangle",
    "star",
    "roundedrectangle",
    NULL
};

shape_type_t
convertToShapeType(const char* shape_name)
{
    int i =0;
    if (shape_name == NULL)
        return (shape_type_t) 0;
    while (gShapes[i] != NULL) {
        if (strcmp(gShapes[i], shape_name) == 0)
            return (shape_type_t) (i);
        i++;
    }
    errx(1, "Unknown shape type '%s' specified", shape_name);
}


int star_points[11][2] = {
    { 0, 85 },
    { 75, 75 },
    { 100, 10 },
    { 125, 75 },
    { 200, 85 },
    { 150, 125 },
    { 160, 190 },
    { 100, 150 },
    { 40, 190 },
    { 50, 125 },
    { 0, 85 }
};

void
shape_init(shapes_t *shape)
{
    memset(shape, 0, sizeof(shapes_t));
}

void
shape_copy(const shapes_t *src, shapes_t *dest)
{
    *dest = *src;
}

void
shape_randomize(shapes_t *shape)
{
    // TODO
}

void
generate_random_color(double &red, double &green, double &blue, double &alpha)
{
    red = (double)rand()/RAND_MAX;
    green = (double)rand()/RAND_MAX;
    blue = (double)rand()/RAND_MAX;
    alpha = (double)rand()/RAND_MAX;
}

shape_type_t
generate_random_shape()
{
    return (shape_type_t) ((((double)(CB_SHAPE_END-1) * rand())/RAND_MAX) + 1);
}

fill_type_t
generate_random_fill_type()
{
    return (fill_type_t) ((((double)(CB_FILL_END-1) * rand())/RAND_MAX) + 1);
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
