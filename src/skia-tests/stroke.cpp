/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkRect.h>
#include <SkDashPathEffect.h>
#include <SkParsePath.h>

#include "kinetics.h"
#include "caskbench.h"
#include "skia-shapes.h"

static kinetics_t *particles;
static int element_spacing;
static int num_x_elements;
static int num_y_elements;

SkAutoTUnref<SkPathEffect> fPE;
static SkRect r;
static int line_length;

static void drawShape(caskbench_context_t *ctx,double x,double y,kinetics_t *particles=NULL)
{
    cairo_t *cr = ctx->cairo_cr;
    int old_x, old_y,old_width, old_height;
    old_width = ctx->shape_args.width;
    old_height = ctx->shape_args.height;

    int i,shape,p;
    double r;
    r = particles?50:0.9 * element_spacing /2;
    if(!ctx->shape_args.shape_id)
        shape = ((4.0 * rand())/RAND_MAX) +1;
    else
        shape = ctx->shape_args.shape_id ;

    switch (shape) {
    case 1:
        // Circle
        ctx->shape_args.center_x = x+r;
        ctx->shape_args.center_y = y+r;
        ctx->shape_args.radius = r;
        skiaShapes[CB_SHAPE_CIRCLE](ctx,&ctx->shape_args);
        break;

    case 2:
        // Rectangle
        ctx->shape_args.center_x =  x;
        ctx->shape_args.center_y = y;
        ctx->shape_args.width = (ctx->shape_args.center_x) +((ctx->shape_args.width)?ctx->shape_args.width:2*r);
        ctx->shape_args.height = (ctx->shape_args.center_y) + ((ctx->shape_args.height)?ctx->shape_args.height:2*r);
        skiaShapes[CB_SHAPE_RECTANGLE](ctx,&ctx->shape_args);
        ctx->shape_args.width = old_width;
        ctx->shape_args.height = old_height;
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
        skiaShapes[CB_SHAPE_TRIANGLE] (ctx,&ctx->shape_args);
        free (ctx->shape_args.points);
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
        skiaShapes[CB_SHAPE_STAR] (ctx,&ctx->shape_args);
        free (ctx->shape_args.points);
        break;

    default:
        break;
    }
    ctx->skia_canvas->flush();
}

static void draw_stroke(caskbench_context_t *ctx,SkCanvas* canvas,kinetics_t* particles) {
    int i,j,x,y;
    for (j=0; j<num_y_elements; j++) {
        y = particles?particles->y : j * element_spacing;
        ctx->skia_paint->setStrokeJoin((SkPaint::Join)(j % 5));
        for (i=0; i<num_x_elements; i++) {
            x = particles?particles->x : i * element_spacing;
            ctx->skia_paint->setStrokeCap((SkPaint::Cap)(i % 5));
            skiaRandomizeColor(ctx);
            drawShape(ctx, x,y);
        }
    }
}

int
sk_setup_stroke(caskbench_context_t *ctx)
{
    SkScalar vals[] = { SkIntToScalar(4), SkIntToScalar(2) };
    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    ctx->skia_paint->setAntiAlias(true);
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    ctx->skia_paint->setStrokeWidth(ctx->shape_args.stroke_width?ctx->shape_args.stroke_width:5);
    ctx->skia_paint->setStrokeJoin(ctx->shape_args.join_style?(SkPaint::Join)(ctx->shape_args.join_style % 5):SkPaint::kDefault_Join);
    ctx->skia_paint->setStrokeCap(ctx->shape_args.cap_style?(SkPaint::Cap)(ctx->shape_args.cap_style % 5):SkPaint::kDefault_Cap);
#if USE_LEGACY_SKIA_SRA
    fPE.reset(new SkDashPathEffect(vals, 4, 0));
#else
    fPE.reset(SkDashPathEffect::Create(vals, 4, 0));
#endif
    ctx->skia_paint->setPathEffect(fPE);

    return 1;
}

void
sk_teardown_stroke(void)
{
}

int
sk_test_stroke(caskbench_context_t *ctx)
{
    /* Animation */
    if(ctx->shape_args.animation)
    {
        int num_particles = ctx->shape_args.animation;
        particles = (kinetics_t *) malloc (sizeof (kinetics_t) * num_particles);
        int i,j ;
        for (i = 0; i < num_particles; i++)
            kinetics_init (&particles[i]);

        for (j=0;j<num_particles;j++){
            ctx->skia_canvas->drawColor(SK_ColorWHITE);
            for (i = 0; i < num_particles; i++) {
                kinetics_update(&particles[i], 0.3);
                skiaRandomizeColor(ctx);
                //draw_stroke(ctx,ctx->skia_canvas,&particles[i]);
                drawShape(ctx,particles[i].x,particles[i].y,&particles[i]);
            }
        }
    }
    /* Static clip */
    else
        ctx->shape_args.multi_shapes?draw_stroke(ctx,ctx->skia_canvas,NULL):drawShape(ctx,ctx->shape_args.center_x?ctx->shape_args.center_x:100,ctx->shape_args.center_y?ctx->shape_args.center_y:100);
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
