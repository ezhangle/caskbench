#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include "caskbench.h"
#include "kinetics.h"
#include "cairo-shapes.h"
static int element_spacing;
static int num_x_elements;
static int num_y_elements;
static kinetics_t *cairo_particles;
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

static kinetics_t *particles;
    cairo_surface_t *image;
int
ca_setup_clip(caskbench_context_t *ctx)
{
    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;
    return 1;
}

void
ca_teardown_clip(void)
{
}

void drawShape(caskbench_context_t *ctx,double x,double y,double clipr=0,bool isClip=false)
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
                ctx->shape_args.radius = isClip?clipr:r;
                cairoShapes[Circle](ctx,&ctx->shape_args);
                break;

            case 2:
                // Rectangle
                ctx->shape_args.centre_x =  x;
                ctx->shape_args.centre_y = y;
                ctx->shape_args.width = (ctx->shape_args.width)?ctx->shape_args.width:2*(isClip?clipr:r);
                ctx->shape_args.height = (ctx->shape_args.height)?ctx->shape_args.height:2*(isClip?clipr:r);
                cairoShapes[Rectangle](ctx,&ctx->shape_args);

                break;

            case 3:
                // Triangle
                ctx->shape_args.numpoints = 3;
                ctx->shape_args.points = (double (*)[2]) malloc(ctx->shape_args.numpoints*2*(sizeof(double)));
                ctx->shape_args.points[0][0] = x;
                ctx->shape_args.points[0][1] = y+2*(isClip?clipr:r);
                ctx->shape_args.points[1][0] = 2*(isClip?clipr:r);
                ctx->shape_args.points[1][1] = 0;
                ctx->shape_args.points[2][0] = -(isClip?clipr:r);
                ctx->shape_args.points[2][1] = -2*(isClip?clipr:r);
                cairoShapes[Triangle] (ctx,&ctx->shape_args);
                free (ctx->shape_args.points);
                cairo_fill (cr);
                break;

            case 4:
                // Star
                ctx->shape_args.numpoints = 10;
                ctx->shape_args.points = (double (*)[2]) malloc(ctx->shape_args.numpoints*2*(sizeof(double)));

                for (p = 0; p < 10; p++ ) {
                    int px = x + 2*(isClip?clipr:r) * star_points[p][0]/200.0;
                    int py = y + 2*(isClip?clipr:r) * star_points[p][1]/200.0;
                    ctx->shape_args.points[p][0] = px;
                    ctx->shape_args.points[p][1] = py;
                }
                cairoShapes[Star] (ctx,&ctx->shape_args);
                cairo_fill (cr);
                free (ctx->shape_args.points);
                break;

            default:
                break;
        }
}
void drawClip(caskbench_context_t *ctx,kinetics_t *particles)
{

    int i, r,j;
    int  w, h;
    double x,y;
    cairo_t *cr = ctx->cairo_cr;
    image = cairo_image_surface_create_from_png ("skia-roundrect.png");
    for (j=0; j<num_y_elements; j++) {
        y = particles?particles->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            x = particles?particles->x : i * element_spacing;
            randomize_color(cr);
            drawShape(ctx,x,y,30,true);
            cairo_clip (cr);
            drawShape(ctx,x,y,false);
            cairo_fill (cr);

            cairo_new_path (cr); /* path not consumed by clip()*/
            cairo_set_source_surface (cr, image, 0, 0);
            cairo_paint (cr);
            cairo_reset_clip(cr);
        }
    }
    cairo_surface_destroy (image);
}

int
ca_test_clip(caskbench_context_t *ctx)
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
                   drawClip(ctx,&particles[i]);
               }
           }

    }
    else
        drawClip(ctx,NULL);
    return 1;
}
