/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>

#include "caskbench.h"
#include "kinetics.h"
#include "cairo-shapes.h"

static int element_spacing;
static int num_x_elements;
static int num_y_elements;
static kinetics_t *particles;
const double dashes[] = { 2, 4 };
static int line_length;

int
ca_setup_stroke(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;
    line_length = 60;
    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;
    cairo_set_line_width(cr,ctx->shape_defaults.stroke_width?ctx->shape_defaults.stroke_width:5);
    cairo_set_line_cap(cr,ctx->shape_defaults.cap_style?(cairo_line_cap_t)(ctx->shape_defaults.cap_style % 3):CAIRO_LINE_CAP_BUTT);
    cairo_set_line_join(cr,ctx->shape_defaults.join_style?(cairo_line_join_t)(ctx->shape_defaults.join_style % 3):CAIRO_LINE_JOIN_MITER);
    cairo_set_dash (cr, dashes, sizeof (dashes) / sizeof (dashes[0]), 0);
    return 1;
}

void
ca_teardown_stroke(void)
{
}

static void drawShape(caskbench_context_t *ctx,double x,double y)
{
    cairo_t *cr = ctx->cairo_cr;
    int i, r, p;
    shape_type_t shape;
    r = 0.9 * element_spacing /2;
    if(ctx->shape_defaults.shape_type == CB_SHAPE_NONE)
        shape = (shape_type_t) (((4.0 * rand())/RAND_MAX) + 1);
    else
        shape = ctx->shape_defaults.shape_type;
    switch (shape) {
    case 1:
        // Circle
        ctx->shape_defaults.x = x+r;
        ctx->shape_defaults.y = y+r;
        ctx->shape_defaults.radius = r;
        cairoShapes[CB_SHAPE_CIRCLE](ctx,&ctx->shape_defaults);
        break;

    case 2:
        // Rectangle
        ctx->shape_defaults.x =  x;
        ctx->shape_defaults.y = y;
        ctx->shape_defaults.width = (ctx->shape_defaults.width)?ctx->shape_defaults.width:2*r;
        ctx->shape_defaults.height = (ctx->shape_defaults.height)?ctx->shape_defaults.height:2*r;
        cairoShapes[CB_SHAPE_RECTANGLE](ctx,&ctx->shape_defaults);
        break;

    case 3:
        // Triangle
        cairoShapes[CB_SHAPE_TRIANGLE] (ctx,&ctx->shape_defaults);
        //cairo_fill (cr);
        break;

    case 4:
        // Star
        cairoShapes[CB_SHAPE_STAR] (ctx,&ctx->shape_defaults);
        //cairo_fill (cr);
        break;

    default:
        break;
    }
}

void drawStroke(caskbench_context_t *ctx,kinetics_t *particles)
{
    int i, r,j;
    int  w, h;
    double x,y;
    cairo_t *cr = ctx->cairo_cr;

    for (j=0; j<num_y_elements; j++) {
        y = particles?particles->y : j * element_spacing;
        cairo_set_line_join(cr,(cairo_line_join_t)(j % 3));
        for (i=0; i<num_x_elements; i++) {
            x = particles?particles->x : i * element_spacing;
            cairo_set_line_cap(cr,(cairo_line_cap_t)(i % 3));
            cairoRandomizeColor(ctx);
            drawShape(ctx,x,y);
            cairo_stroke (cr);
        }
    }
}

int
ca_test_stroke(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;
    if(ctx->shape_defaults.animation)
    {
        int num_particles = ctx->shape_defaults.animation;
        double start_frame, stop_frame, delta;
        particles = (kinetics_t *) malloc (sizeof (kinetics_t) * num_particles);
        int i,j;
        for (i = 0; i < num_particles; i++)
            kinetics_init (&particles[i]);
        delta = 0;

        for (j=0;j<num_particles;j++){
            cairo_set_source_rgb (cr, 1, 1, 1);
            cairo_rectangle (cr, 0, 0, ctx->canvas_width ,ctx->canvas_height);
            cairo_fill (cr);

            for (i = 0; i < num_particles; i++) {
                kinetics_update(&particles[i], 0.1);
                drawStroke(ctx,&particles[i]);
            }
        }
    }
    else
        drawShape(ctx,ctx->shape_defaults.x?ctx->shape_defaults.x:100.0,ctx->shape_defaults.y?ctx->shape_defaults.y:100.0);

    return 1;
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
