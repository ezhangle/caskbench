/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/
#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkOSFile.h>
#include <SkImageDecoder.h>

#include "kinetics.h"
#include "caskbench.h"
#include "skia-shapes.h"

static kinetics_t *particles;
static int element_spacing;
static int num_x_elements;
static int num_y_elements;


static bool
draw_clip_tests (caskbench_context_t *ctx,SkCanvas* canvas,kinetics_t* particles) {
    int i,j,x,y;

    for (j=0; j<num_y_elements; j++) {
        y = particles?particles->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            x = particles?particles->x : i * element_spacing;
            skiaRandomizePaintColor(ctx);
            /*
            if (!draw_square(ctx,canvas, x,y))
                return false;
            */
        }
    }
    return true;
}

int
sk_setup_clip(caskbench_context_t *ctx)
{
    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    return 1;
}

void
sk_teardown_clip(void)
{
}

int
sk_test_clip(caskbench_context_t *ctx)
{
    /* Animation */
    if(ctx->shape_defaults.animation)
    {
        int num_particles = ctx->shape_defaults.animation;
        double start_frame, stop_frame;
        particles = (kinetics_t *) malloc (sizeof (kinetics_t) * num_particles);
        int i,j ;
        for (i = 0; i < num_particles; i++)
            kinetics_init (&particles[i]);

        for (j=0;j<num_particles;j++){
            ctx->skia_canvas->drawColor(SK_ColorBLACK);
            for (i = 0; i < num_particles; i++) {
                kinetics_update(&particles[i], 0.1);
                if (!draw_clip_tests(ctx,ctx->skia_canvas,&particles[i]))
                    return 0;
            }
        }
    }
    /* Static clip */
    else {
        if (!draw_clip_tests(ctx,ctx->skia_canvas,NULL))
            return 0;
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
