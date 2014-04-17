#include <kinetics.h>

void
generate_color (double *color)
{
    int i;

    for (i = 0; i <= 3; i++)
        color[i] = (int) (drand48 () * 2) / 2.0;
}

void kinetics_destroy (kinetics_t *kinetics)
{
    free(kinetics);
}

void kinetics_init (kinetics_t *kinetics)
{
    kinetics->angle = drand48 () * 2 * M_PI;
    kinetics->velocity = MAX_VELOCITY / 8.0 * 7 * drand48 () + MAX_VELOCITY / 8.0;
    kinetics->x = WIDTH / 2;
    kinetics->y = HEIGHT / 2;
    generate_color (kinetics->color);

    kinetics->x_radius = drand48 () * 10 + 5;
    kinetics->y_radius = drand48 () * 10 + 5;
    kinetics->x_offset = drand48 () * 10;
    kinetics->y_offset = drand48 () * 10;
    kinetics->width = drand48 () * 120 + 30;
    kinetics->height = drand48 () * 120 + 30;

    generate_color (kinetics->shadow_color);
    kinetics->rotation = drand48 () * 2 * M_PI;
    kinetics->line_width = 20; //drand48() * 20;

    /* inset shadow */
    kinetics->shadow_color[0] = 0;
    kinetics->shadow_color[1] = 0;
    kinetics->shadow_color[2] = 0;
    kinetics->shadow_color[3] = 1;
    kinetics->shadow_x_blur = 5;
    kinetics->shadow_y_blur = 2;

    kinetics->x_offset = 6;
    kinetics->y_offset = 1;

    /* spread */
    kinetics->spread_color[0] = 1;
    kinetics->spread_color[1] = 1;
    kinetics->spread_color[2] = 1;
    kinetics->spread_color[3] = 1;
    kinetics->spread_x_blur = 5;
    kinetics->spread_y_blur = 2;
    kinetics->spread_line_width = 3;

    /* drop shadow */
    kinetics->drop_color[0] = 0;
    kinetics->drop_color[1] = 0;
    kinetics->drop_color[2] = 0;
    kinetics->drop_color[3] = 0.5;
    kinetics->drop_x_blur = 10;
    kinetics->drop_y_blur = 8;
    kinetics->drop_x_offset = -30;
    kinetics->drop_y_offset = -5;
    kinetics->drop_line_width = kinetics->line_width;
}

void kinetics_update (kinetics_t *kinetics, double delta)
{
    double x = kinetics->x + cos (kinetics->angle) * kinetics->velocity * delta;
    double y = kinetics->y + sin (kinetics->angle) * kinetics->velocity * delta;
    double w = kinetics->width/2;
    double height = kinetics->height/2;

    if (x + w > WIDTH) {
        if (kinetics->angle >= 0 && kinetics->angle < M_PI / 2)
            kinetics->angle =  M_PI - kinetics->angle;
        else if (kinetics->angle > M_PI / 2 * 3)
            kinetics->angle -= (kinetics->angle - M_PI /2 * 3) * 2;
        x = WIDTH - w;
    }

    if (x - w < 0) {
        if (kinetics->angle > M_PI / 2 && kinetics->angle <  M_PI)
            kinetics->angle = M_PI - kinetics->angle;
        else if (kinetics->angle > M_PI && kinetics->angle < M_PI / 2 * 3)
            kinetics->angle += (M_PI / 2 * 3 - kinetics->angle) * 2;
        x = w;
    }

    if (y + w > HEIGHT) {
        if (kinetics->angle > 0 && kinetics->angle < 2 * M_PI)
            kinetics->angle = M_PI * 2 - kinetics->angle;
        y = HEIGHT - w;
    }

    if (y - w < 0) {
        if (kinetics->angle > M_PI && kinetics->angle < M_PI * 2)
            kinetics->angle = kinetics->angle - (kinetics->angle - M_PI) * 2;
        y = w;
    }

    kinetics->x = x;
    kinetics->y = y;

    kinetics->rotation += M_PI / 9;
    if (kinetics->rotation > 2 * M_PI)
        kinetics->rotation -= 2 * M_PI;
}
