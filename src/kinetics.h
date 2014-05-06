#ifndef __KINETICS_H__
#define __KINETICS_H__

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
    double width;
    double height;
    double line_width;
} kinetics_t;

void kinetics_init (kinetics_t *kinetics);
void kinetics_update (kinetics_t *kinetics, double delta);
void kinetics_destroy(kinetics_t *kinetics);

#endif // __KINETICS_H__
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
