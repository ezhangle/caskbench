#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define WIDTH 800
#define HEIGHT 400

#define MAX_VELOCITY WIDTH / 2

#include "kinetics.h"

void
kinetics_destroy (kinetics_t *kinetics)
{
    free(kinetics);
}

void
kinetics_init (kinetics_t *kinetics)
{
    kinetics->angle = drand48 () * 2 * M_PI;
    kinetics->velocity = MAX_VELOCITY / 8.0 * 7 * drand48 () + MAX_VELOCITY / 8.0;
    kinetics->x = WIDTH / 2;
    kinetics->y = HEIGHT / 2;
    
    kinetics->x_radius = drand48 () * 10 + 5;
    kinetics->y_radius = drand48 () * 10 + 5;
    kinetics->width = drand48 () * 120 + 30;
    kinetics->height = drand48 () * 120 + 30;

    kinetics->rotation = drand48 () * 2 * M_PI;
    kinetics->line_width = 20; //drand48() * 20;
}

void
kinetics_update (kinetics_t *kinetics, double delta)
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
