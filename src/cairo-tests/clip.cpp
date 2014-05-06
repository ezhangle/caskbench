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
static kinetics_t *cairo_particles;

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
    ctx->shape_args.center_x = x;
    ctx->shape_args.center_y = y;
    ctx->shape_args.radius = isClip?clipr:r;
    ctx->shape_args.width = (ctx->shape_args.width)?ctx->shape_args.width:2*(isClip?clipr:r);
    ctx->shape_args.height = (ctx->shape_args.height)?ctx->shape_args.height:2*(isClip?clipr:r);
    cairoShapes[(shape-1)%4](ctx,&ctx->shape_args);
}
void drawClip(caskbench_context_t *ctx,kinetics_t *particles)
{
    int i, r,j;
    int  w, h;
    double x,y;
    cairo_t *cr = ctx->cairo_cr;
    image = cairo_image_surface_create_from_png (ctx->shape_args.image_path);
    for (j=0; j<num_y_elements; j++) {
        y = particles?particles->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            x = particles?particles->x : i * element_spacing;
            cairoRandomizeColor(ctx);
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
