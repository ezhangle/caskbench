/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "kinetics.h"

void
kinetics_destroy (kinetics_t *kinetics)
{
    free(kinetics);
}

void
kinetics_init (kinetics_t *kinetics, const caskbench_context_t *ctx)
{
    int max_velocity = ctx->canvas_width / 2;
    kinetics->angle = drand48 () * 2 * M_PI;
    kinetics->velocity = max_velocity / 8.0 * 7 * drand48 () + max_velocity / 8.0;
    kinetics->x = ctx->canvas_width / 2;
    kinetics->y = ctx->canvas_height / 2;

    kinetics->x_radius = drand48 () * 10 + 5;
    kinetics->y_radius = drand48 () * 10 + 5;
    kinetics->canvas_width = ctx->canvas_width;
    kinetics->canvas_height = ctx->canvas_height;
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

    if (x + w > kinetics->canvas_width) {
        if (kinetics->angle >= 0 && kinetics->angle < M_PI / 2)
            kinetics->angle =  M_PI - kinetics->angle;
        else if (kinetics->angle > M_PI / 2 * 3)
            kinetics->angle -= (kinetics->angle - M_PI /2 * 3) * 2;
        x = kinetics->canvas_width - w;
    }

    if (x - w < 0) {
        if (kinetics->angle > M_PI / 2 && kinetics->angle <  M_PI)
            kinetics->angle = M_PI - kinetics->angle;
        else if (kinetics->angle > M_PI && kinetics->angle < M_PI / 2 * 3)
            kinetics->angle += (M_PI / 2 * 3 - kinetics->angle) * 2;
        x = w;
    }

    if (y + w > kinetics->canvas_height) {
        if (kinetics->angle > 0 && kinetics->angle < 2 * M_PI)
            kinetics->angle = M_PI * 2 - kinetics->angle;
        y = kinetics->canvas_height - w;
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
