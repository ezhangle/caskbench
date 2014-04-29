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
    int i, j, x, y, r, p, shape;
    cairo_t *cr = ctx->cairo_cr;
    r = 0.9 * element_spacing /2;
    int old_x, old_y,old_width, old_height;
    cairo_pattern_t *pattern;
    cairo_surface_t *image;
    double red, green, blue, alpha;
    static const double dashed3[] = {1.0};
    static const double dashed2[] = {14.0, 6.0};
    int px, py;
    cairo_path_t *newPath;
    shape = ctx->shape_args.shape_id;
    if (!shape)
    {
        shape = ((4.0 * rand())/RAND_MAX) + 1;
        if (!ctx->shape_args.multi_shapes)
            shape = 2;
    }

    if (shape)
    {
        if (!(ctx->shape_args.center_x && ctx->shape_args.center_y))
        {
            ctx->shape_args.center_x = x = ctx->canvas_width/2;
            ctx->shape_args.center_y = y = ctx->canvas_height/2;
        }
        if (!ctx->shape_args.multi_shapes)
            if (!(ctx->shape_args.width && ctx->shape_args.height))
            {
                ctx->shape_args.width = 100;
                ctx->shape_args.height = 50;
            }
        ctx->shape_args.radius = r;
    }


    if (!ctx->shape_args.animation && !ctx->shape_args.multi_shapes)
    {
        num_x_elements = 1;
        num_y_elements = 1;

        r = 40;
        old_x = ctx->shape_args.center_x;
        old_y = ctx->shape_args.center_y;
        old_width = ctx->shape_args.width;
        old_height = ctx->shape_args.height;
    }

    for (j=0; j<num_y_elements; j++) {
        y = particles?particles->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            x = particles?particles->x : i * element_spacing;

            //Options for fill,gradient and transparency
            if ((ctx->shape_args.fill_type == none) || (ctx->shape_args.fill_type == solid))
            {
                if (ctx->shape_args.red > 0 || ctx->shape_args.blue > 0 || ctx->shape_args.green > 0 || ctx->shape_args.alpha > 0)
                {
                    cairo_set_source_rgba(cr, ctx->shape_args.red, ctx->shape_args.green, ctx->shape_args.blue, ctx->shape_args.alpha?ctx->shape_args.alpha:1);
                }
                else
                    cairoRandomizeColor(ctx);
            }
            else if ((ctx->shape_args.fill_type == linearGradient) || (ctx->shape_args.fill_type == radialGradient))
            {
                if (ctx->shape_args.red > 0 || ctx->shape_args.blue > 0 || ctx->shape_args.green > 0 || ctx->shape_args.alpha > 0)
                {
                    red = ctx->shape_args.red;
                    green = ctx->shape_args.green;
                    blue = ctx->shape_args.blue;
                    alpha = ctx->shape_args.alpha;
                }
                else
                {
                    red = (double)rand()/RAND_MAX;
                    green = (double)rand()/RAND_MAX;
                    blue = (double)rand()/RAND_MAX;
                    alpha = (double)rand()/RAND_MAX;
                }
            }
            else if (ctx->shape_args.fill_type == herringbonePattern){}
            else if ((ctx->shape_args.fill_type == imagePattern) && (ctx->shape_args.image_path))
            {
                cairo_matrix_t   matrix;

                image = cairo_image_surface_create_from_png (ctx->shape_args.image_path);
                int w = cairo_image_surface_get_width (image);
                int h = cairo_image_surface_get_height (image);

                pattern = cairo_pattern_create_for_surface (image);
                cairo_set_source (cr, pattern);
            }

            switch (shape) {
            case 1:
                // Circle
                ctx->shape_args.center_x = (ctx->shape_args.multi_shapes || ctx->shape_args.animation)?x+r:ctx->shape_args.center_x;
                ctx->shape_args.center_y = (ctx->shape_args.multi_shapes || ctx->shape_args.animation)?y+r:ctx->shape_args.center_y;
                ctx->shape_args.radius = r;

                if (ctx->shape_args.fill_type != none)
                {
                    if (ctx->shape_args.fill_type == radialGradient)
                        pattern = cairo_pattern_create_radial (ctx->shape_args.center_x, ctx->shape_args.center_y ,r,ctx->shape_args.center_x, ctx->shape_args.center_y,0 );
                    else if (ctx->shape_args.fill_type == linearGradient)
                        pattern = cairo_pattern_create_linear (0, ctx->shape_args.center_y-r ,0, ctx->shape_args.center_y+r );

                    if (ctx->shape_args.fill_type == linearGradient)
                       cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
                    if ((ctx->shape_args.fill_type == linearGradient) || (ctx->shape_args.fill_type == radialGradient))
                    {
                        cairo_pattern_add_color_stop_rgba (pattern, 0, red, green, blue, alpha);
                        cairo_pattern_add_color_stop_rgba (pattern, 1, 0.78, 0.78, 0.78, 0.78);
                        cairo_set_source (cr, pattern);
                    }
                }
                cairoShapes[Circle] (ctx,&ctx->shape_args);

                break;

            case 2:
                // Rectangle
                ctx->shape_args.center_x =  (ctx->shape_args.multi_shapes || ctx->shape_args.animation)?x:ctx->shape_args.center_x;
                ctx->shape_args.center_y = (ctx->shape_args.multi_shapes|| ctx->shape_args.animation)?y:ctx->shape_args.center_y;
                ctx->shape_args.width = (ctx->shape_args.width)?ctx->shape_args.width:2*r;
                ctx->shape_args.height = (ctx->shape_args.height)?ctx->shape_args.height:2*r;

                if (ctx->shape_args.fill_type != none)
                {
                    if (ctx->shape_args.fill_type == radialGradient)
                        pattern = cairo_pattern_create_radial (ctx->shape_args.center_x+(ctx->shape_args.width/2),
                                                               ctx->shape_args.center_y+(ctx->shape_args.height/2),
                                                               ctx->shape_args.height/2, ctx->shape_args.center_x+(ctx->shape_args.width/2),
                                                               ctx->shape_args.center_y+(ctx->shape_args.height/2),
                                                               0/*ctx->shape_args.height/2 */);
                    else if (ctx->shape_args.fill_type == linearGradient)
                        pattern = cairo_pattern_create_linear (0, ctx->shape_args.center_y, 0,
                                                               ctx->shape_args.center_y+ctx->shape_args.height );

                    if (ctx->shape_args.fill_type == linearGradient)
                       cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
                    if ((ctx->shape_args.fill_type == linearGradient) || (ctx->shape_args.fill_type == radialGradient))
                    {
                        cairo_pattern_add_color_stop_rgba (pattern, 0, red, green, blue, alpha);
                        cairo_pattern_add_color_stop_rgba (pattern, 1, 0.78, 0.78, 0.78, 0.78);
                        cairo_set_source (cr, pattern);
                    }
                }
                cairoShapes[Rectangle] (ctx,&ctx->shape_args);

                break;

            case 3:
                // Triangle
                if ((!ctx->shape_args.multi_shapes && !ctx->shape_args.animation))
                {
                    x = ctx->shape_args.center_x;
                    y = ctx->shape_args.center_y;
                }

                ctx->shape_args.center_x = x;
                ctx->shape_args.center_y = y;

                if (ctx->shape_args.fill_type != none)
                {
                    if (ctx->shape_args.fill_type == radialGradient)
                        pattern = cairo_pattern_create_radial (x, y+2*r, r, x, y+2*r,0/*2*r*/);
                    else if (ctx->shape_args.fill_type == linearGradient)
                        pattern = cairo_pattern_create_linear (0, (y+2*r) ,0, (y+2*r)+(4*r) );

                    if (ctx->shape_args.fill_type == linearGradient)
                        cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
                    if ((ctx->shape_args.fill_type == linearGradient) || (ctx->shape_args.fill_type == radialGradient))
                    {
                        cairo_pattern_add_color_stop_rgba (pattern, 0, red, green, blue, alpha);
                        cairo_pattern_add_color_stop_rgba (pattern, 1, 0.78, 0.78, 0.78, 0.78);
                        cairo_set_source (cr, pattern);
                    }

                }
                cairoShapes[Triangle] (ctx,&ctx->shape_args);

                break;

            case 4:
                // Star
                if ((!ctx->shape_args.multi_shapes && !ctx->shape_args.animation))
                {
                    x = ctx->shape_args.center_x;
                    y = ctx->shape_args.center_y;
                }

                ctx->shape_args.center_x = x;
                ctx->shape_args.center_y = y;

                px = x + 2*r * star_points[0][0]/200.0;
                py = y + 2*r * star_points[0][1]/200.0;

                if (ctx->shape_args.fill_type != none)
                {
                    if (ctx->shape_args.fill_type == radialGradient)
                        pattern = cairo_pattern_create_radial (px, py, r, x, py,0/*r*/);
                    else if (ctx->shape_args.fill_type == linearGradient)
                        pattern = cairo_pattern_create_linear (0, py-r ,0, py+(r) );


                    if (ctx->shape_args.fill_type == linearGradient)
                        cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
                    if ((ctx->shape_args.fill_type == linearGradient) || (ctx->shape_args.fill_type == radialGradient))
                    {
                        cairo_pattern_add_color_stop_rgba (pattern, 0, red, green, blue, alpha);
                        cairo_pattern_add_color_stop_rgba (pattern, 1, 0.78, 0.78, 0.78, 0.78);
                        cairo_set_source (cr, pattern);
                    }
                }
                cairoShapes[Star] (ctx,&ctx->shape_args);

                break;

            default:
                break;
            }
            newPath = cairo_copy_path(cr);
            cairo_fill (cr);
            if (ctx->shape_args.stroke_width)
            {
                cairo_new_path(cr);
                cairo_append_path(cr, newPath);
                cairo_set_line_width(cr, ctx->shape_args.stroke_width);
                cairo_set_line_cap(cr, (cairo_line_cap_t)ctx->shape_args.cap_style);
                if (ctx->shape_args.dash_style == 0)
                {
                    cairo_set_dash(cr, dashed3, 1, 0);
                }
                else if (ctx->shape_args.dash_style == 1)
                {
                    cairo_set_dash(cr, dashed2, 2, 0);
                }

                cairo_set_line_join(cr,  (cairo_line_join_t)ctx->shape_args.join_style);
                if (ctx->shape_args.stroke_red > 0 || ctx->shape_args.stroke_blue > 0 || ctx->shape_args.stroke_green > 0 || ctx->shape_args.stroke_alpha > 0)
                {
                    cairo_set_source_rgba (cr, ctx->shape_args.stroke_red, ctx->shape_args.stroke_green, ctx->shape_args.stroke_blue,
                                           ctx->shape_args.stroke_alpha?ctx->shape_args.stroke_alpha:1);
                }
                else
                    cairoRandomizeColor(ctx);

                cairo_stroke(cr);
            }
            if ((ctx->shape_args.fill_type != none) && (ctx->shape_args.fill_type != solid))
            {
                if (pattern != NULL)
                    cairo_pattern_destroy(pattern);
                if ((image != NULL) && (ctx->shape_args.fill_type == imagePattern))
                    cairo_surface_destroy (image);
            }
        }
    }

    if (!ctx->shape_args.animation && !ctx->shape_args.multi_shapes)
    {
        ctx->shape_args.center_x = old_x;
        ctx->shape_args.center_y = old_y;
        ctx->shape_args.width = old_width;
        ctx->shape_args.height = old_height;
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
    for (int i = 0;i < ctx->size; i++) {
        //Animation / Kinematics of single or multi shape
        if (ctx->shape_args.animation)
        {
            int num_particles = ctx->shape_args.animation;
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
