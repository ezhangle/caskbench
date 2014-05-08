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
#include <cairo.h>
#include <math.h>

#include "cairo-shapes.h"
#include "caskbench.h"
#include "kinetics.h"
#include "unistd.h"

static int element_spacing;
static int num_x_elements;
static int num_y_elements;
static kinetics_t *cairo_particles;

void drawshapes(caskbench_context_t *ctx,kinetics_t *particles)
{
    int i, j, x, y, r, p;
    shape_type_t shape;
    cairo_t *cr = ctx->cairo_cr;
    r = 0.9 * element_spacing /2;
    int old_x, old_y,old_width, old_height;
    cairo_pattern_t *pattern;
    cairo_surface_t *image;
    double red, green, blue, alpha;
    static const double dashed3[] = {1.0};
    static const double dashed2[] = {14.0, 6.0};
    cairo_path_t *newPath;
    shape = ctx->shape_defaults.shape_type;
    if(shape == CB_SHAPE_NONE)
    {
        shape = (shape_type_t) (((4.0 * rand())/RAND_MAX) + 1);
        if(!ctx->shape_defaults.multi_shapes)
            shape = CB_SHAPE_RECTANGLE;
    } else {
        if(!(ctx->shape_defaults.x && ctx->shape_defaults.y))
        {
            ctx->shape_defaults.x = x = ctx->canvas_width/2;
            ctx->shape_defaults.y = y = ctx->canvas_height/2;
        }
        if(!ctx->shape_defaults.multi_shapes)
            if(!(ctx->shape_defaults.width && ctx->shape_defaults.height))
            {
                ctx->shape_defaults.width = 100;
                ctx->shape_defaults.height = 50;
            }
        ctx->shape_defaults.radius = r;
    }


    if(!ctx->shape_defaults.animation && !ctx->shape_defaults.multi_shapes)
    {
        num_x_elements = 1;
        num_y_elements = 1;

        r = 40;
        old_x = ctx->shape_defaults.x;
        old_y = ctx->shape_defaults.y;
        old_width = ctx->shape_defaults.width;
        old_height = ctx->shape_defaults.height;
    }

    for (j=0; j<num_y_elements; j++) {
        y = particles?particles->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            x = particles?particles->x : i * element_spacing;

            //Options for fill,gradient and transparency
            if (ctx->shape_defaults.fill_type == CB_FILL_NONE)
            {
                if(ctx->shape_defaults.red > 0 || ctx->shape_defaults.blue > 0 || ctx->shape_defaults.green > 0 || ctx->shape_defaults.alpha > 0)
                {
                    cairo_set_source_rgba(cr, (double)ctx->shape_defaults.red, (double)ctx->shape_defaults.green ? (double)ctx->shape_defaults.green:(double)0, ctx->shape_defaults.blue ? (double)ctx->shape_defaults.blue:(double)0, (double)ctx->shape_defaults.alpha ? (double)ctx->shape_defaults.alpha:(double)1);
                }
                else
                    cairoRandomizeColor(ctx);
            }
            else if (ctx->shape_defaults.fill_type == CB_FILL_SOLID)
            {
                if(ctx->shape_defaults.red > 0 || ctx->shape_defaults.blue > 0 || ctx->shape_defaults.green > 0 || ctx->shape_defaults.alpha > 0)
                {
                    cairo_set_source_rgba(cr,  (double)ctx->shape_defaults.red, (double)ctx->shape_defaults.green ? (double)ctx->shape_defaults.green:(double)0, ctx->shape_defaults.blue ? (double)ctx->shape_defaults.blue:(double)0, (double)ctx->shape_defaults.alpha ? (double)ctx->shape_defaults.alpha:(double)1);
                }
                else
                    cairoRandomizeColor(ctx);
            }
            else if (ctx->shape_defaults.fill_type == CB_FILL_LINEAR_GRADIENT || ctx->shape_defaults.fill_type == CB_FILL_RADIAL_GRADIENT)
            {
                red = (double)rand()/RAND_MAX;
                green = (double)rand()/RAND_MAX;
                blue = (double)rand()/RAND_MAX;
                alpha = (double)rand()/RAND_MAX;

            }
            else if(ctx->shape_defaults.fill_type == CB_FILL_HERRINGBONE_PATTERN){}
            else if(ctx->shape_defaults.fill_type == CB_FILL_IMAGE_PATTERN && (ctx->shape_defaults.image_path))
            {
                cairo_matrix_t   matrix;

                image = cairo_image_surface_create_from_png (ctx->shape_defaults.image_path);
                int w = cairo_image_surface_get_width (image);
                int h = cairo_image_surface_get_height (image);

                pattern = cairo_pattern_create_for_surface (image);
                cairo_set_source (cr, pattern);
            }

            if (!ctx->shape_defaults.multi_shapes && !ctx->shape_defaults.animation) {
                x = ctx->shape_defaults.x;
                y = ctx->shape_defaults.y;
            }

            double cx, cy, rr, x1, x2;
            shape_type_t shape_type;
            switch (shape) {
            case 1:
                // Circle
                shape_type = CB_SHAPE_CIRCLE;
                ctx->shape_defaults.x = x;
                ctx->shape_defaults.y = y;
                ctx->shape_defaults.radius = r;

                cx = x;
                cy = y;
                rr = r;
                x1 = y-r;
                x2 = y+r;

                break;

            case 2:
                // Rectangle
                shape_type = CB_SHAPE_RECTANGLE;
                ctx->shape_defaults.x = x;
                ctx->shape_defaults.y = y;
                ctx->shape_defaults.width = (ctx->shape_defaults.width)?ctx->shape_defaults.width:2*r;
                ctx->shape_defaults.height = (ctx->shape_defaults.height)?ctx->shape_defaults.height:2*r;

                cx = x+(ctx->shape_defaults.width/2);
                cy = y+(ctx->shape_defaults.height/2);
                rr = ctx->shape_defaults.height/2;
                x1 = y;
                x2 = y+ctx->shape_defaults.height;

                break;

            case 3:
                // Triangle
                shape_type = CB_SHAPE_TRIANGLE;

                cx = x;
                cy = y + 2*r;
                rr = r;
                x1 = y + 2*r;
                x2 = y + 2*r + 4*r;

                break;

            case 4:
                // Star
                shape_type = CB_SHAPE_STAR;

                cx = x + 2*r * star_points[0][0]/200.0;
                cy = y + 2*r * star_points[0][1]/200.0;
                rr = r;
                x1 = x + 2*r * star_points[p][0]/200.0 - r;
                x2 = y + 2*r * star_points[0][1]/200.0 + r;

                break;

            default:
                // TODO: Should never reach here
                break;
            }

            if (ctx->shape_defaults.fill_type != CB_FILL_NONE)
            {
                if (ctx->shape_defaults.fill_type == CB_FILL_RADIAL_GRADIENT) {
                    pattern = cairo_pattern_create_radial (cx, cy, rr, cx, cy, 0);
                    cairo_pattern_add_color_stop_rgba (pattern, 0, red, green, blue, alpha);
                    cairo_pattern_add_color_stop_rgba (pattern, 1, 0.78, 0.78, 0.78, 0.78);
                    cairo_set_source (cr, pattern);
                } else if (ctx->shape_defaults.fill_type == CB_FILL_LINEAR_GRADIENT) {
                    pattern = cairo_pattern_create_linear (0, x1, 0, x2);
                    cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
                    cairo_pattern_add_color_stop_rgba (pattern, 0, red, green, blue, alpha);
                    cairo_pattern_add_color_stop_rgba (pattern, 1, 0.78, 0.78, 0.78, 0.78);
                    cairo_set_source (cr, pattern);
                }
            }

            cairoShapes[shape_type] (ctx,&ctx->shape_defaults);

            // Reset
            ctx->shape_defaults.width = old_width;
            ctx->shape_defaults.height = old_height;

            newPath = cairo_copy_path(cr);
            cairo_fill (cr);
            if(ctx->shape_defaults.stroke_width)
            {
                cairo_new_path(cr);
                cairo_append_path(cr, newPath);
                cairo_set_line_width(cr, ctx->shape_defaults.stroke_width);
                cairo_set_line_cap(cr, (cairo_line_cap_t)ctx->shape_defaults.cap_style);
                if (ctx->shape_defaults.dash_style == 1)
                {
                    cairo_set_dash(cr, dashed3, 1, 0);
                }
                else if (ctx->shape_defaults.dash_style == 2)
                {
                    cairo_set_dash(cr, dashed2, 2, 0);
                }

                cairo_set_line_join(cr,  (cairo_line_join_t)ctx->shape_defaults.join_style);
                if (ctx->shape_defaults.red > 0 || ctx->shape_defaults.blue > 0 || ctx->shape_defaults.green > 0 || ctx->shape_defaults.alpha > 0)
                {
                    cairo_set_source_rgba(cr,  (double)ctx->shape_defaults.red, (double)ctx->shape_defaults.green ? (double)ctx->shape_defaults.green:(double)0, ctx->shape_defaults.blue ? (double)ctx->shape_defaults.blue:(double)0, (double)ctx->shape_defaults.alpha ? (double)ctx->shape_defaults.alpha:(double)1);
                }
                else
                    cairoRandomizeColor(ctx);

                cairo_stroke(cr);
            }
            if (ctx->shape_defaults.fill_type != CB_FILL_NONE)
            {
                if (pattern != NULL)
                    cairo_pattern_destroy(pattern);
                if (image != NULL)
                    cairo_surface_destroy (image);
            }
        }
    }

    if (!ctx->shape_defaults.animation && !ctx->shape_defaults.multi_shapes)
    {
        ctx->shape_defaults.x = old_x;
        ctx->shape_defaults.y = old_y;
        ctx->shape_defaults.width = old_width;
        ctx->shape_defaults.height = old_height;
    }
}

int
ca_setup_fill(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;

    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    return 1;
}

void
ca_teardown_fill(void)
{
}

int
ca_test_fill(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;
    for(int i = 0;i < ctx->size; i++) {
        //Animation / Kinematics of single or multi shape
        if(ctx->shape_defaults.animation)
        {
            int num_particles = ctx->shape_defaults.animation;
            double start_frame, stop_frame, delta;
            cairo_particles = (kinetics_t *) malloc (sizeof (kinetics_t) * num_particles);
            int i,j;
            for (i = 0; i < num_particles; i++)
                kinetics_init (&cairo_particles[i]);
            delta = 0;

            for (j=0;j<num_particles;j++){
                start_frame = get_tick ();

                cairo_set_source_rgb (cr, 0, 0, 0);
                cairo_rectangle (cr, 0, 0, ctx->canvas_width ,ctx->canvas_height);
                cairo_fill (cr);

                for (i = 0; i < num_particles; i++) {
                    kinetics_update(&cairo_particles[i], 0.1);
                    drawshapes(ctx,&cairo_particles[i]);
                }
                stop_frame = get_tick ();
                delta = stop_frame - start_frame;
            }

        }

        // Drawing of multishape on a grid
        else
        {
            drawshapes(ctx,NULL);
        }
    }
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
