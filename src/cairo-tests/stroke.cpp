#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include "caskbench.h"
#include "kinetics.h"
#include "cairo-shapes.h"
static int element_spacing;
static int num_x_elements;
static int num_y_elements;

static kinetics_t *particles;
const double dashes[] = { 2, 4 };
static int line_length;
static int star_points[11][2] = {
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

int
ca_setup_stroke(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;
    line_length = 60;
    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;
    cairo_set_line_width(cr,ctx->shape_args.stroke_width?ctx->shape_args.stroke_width:5);
    cairo_set_line_cap(cr,ctx->shape_args.cap_style?(cairo_line_cap_t)(ctx->shape_args.cap_style % 3):CAIRO_LINE_CAP_BUTT);
    cairo_set_line_join(cr,ctx->shape_args.join_style?(cairo_line_join_t)(ctx->shape_args.join_style % 3):CAIRO_LINE_JOIN_MITER);
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
    int i, r,shape,p;
    r = 0.9 * element_spacing /2;
        if(!ctx->shape_args.shape_id)
            shape = ((4.0 * rand())/RAND_MAX) +1;
        else
            shape = ctx->shape_args.shape_id ;
        switch (shape) {
            case 1:
                // Circle
                ctx->shape_args.centre_x = x+r;
                ctx->shape_args.centre_y = y+r;
                ctx->shape_args.radius = r;
                cairoShapes[Circle](ctx,&ctx->shape_args);
                break;

            case 2:
                // Rectangle
                ctx->shape_args.centre_x =  x;
                ctx->shape_args.centre_y = y;
                ctx->shape_args.width = (ctx->shape_args.width)?ctx->shape_args.width:2*r;
                ctx->shape_args.height = (ctx->shape_args.height)?ctx->shape_args.height:2*r;
                cairoShapes[Rectangle](ctx,&ctx->shape_args);

                break;

            case 3:
                // Triangle
                ctx->shape_args.numpoints = 3;
                ctx->shape_args.points = (double (*)[2]) malloc(ctx->shape_args.numpoints*2*(sizeof(double)));
                ctx->shape_args.points[0][0] = x;
                ctx->shape_args.points[0][1] = y+2*r;
                ctx->shape_args.points[1][0] = 2*r;
                ctx->shape_args.points[1][1] = 0;
                ctx->shape_args.points[2][0] = -r;
                ctx->shape_args.points[2][1] = -2*r;
                cairoShapes[Triangle] (ctx,&ctx->shape_args);
                free (ctx->shape_args.points);
                //cairo_fill (cr);
                break;

            case 4:
                // Star
                ctx->shape_args.numpoints = 10;
                ctx->shape_args.points = (double (*)[2]) malloc(ctx->shape_args.numpoints*2*(sizeof(double)));

                for (p = 0; p < 10; p++ ) {
                    int px = x + 2*r * star_points[p][0]/200.0;
                    int py = y + 2*r * star_points[p][1]/200.0;
                    ctx->shape_args.points[p][0] = px;
                    ctx->shape_args.points[p][1] = py;
                }
                cairoShapes[Star] (ctx,&ctx->shape_args);
                //cairo_fill (cr);
                free (ctx->shape_args.points);
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
            randomize_color(cr);
            drawShape(ctx,x,y);
            cairo_stroke (cr);

        }
    }
}
int
ca_test_stroke(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;
    if(ctx->shape_args.animation)
    {
        int num_particles = ctx->shape_args.animation;
        double start_frame, stop_frame, delta;
        particles = (kinetics_t *) malloc (sizeof (kinetics_t) * num_particles);
        int i,j ;
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
        //drawStroke(ctx,NULL);
       ctx->shape_args.multi_shapes?drawStroke(ctx,NULL):drawShape(ctx,ctx->shape_args.centre_x?ctx->shape_args.centre_x:100.0,ctx->shape_args.centre_y?ctx->shape_args.centre_y:100.0);


    return 1;
}
