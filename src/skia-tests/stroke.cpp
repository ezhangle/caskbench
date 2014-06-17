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
#include "caskbench_context.h"
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
    int old_x, old_y;
    int i, p;
    double r;
    shape_type_t shape;

    r = particles?50:0.9 * element_spacing /2;
    if(ctx->shape_defaults.shape_type == CB_SHAPE_NONE)
        shape = generate_random_shape();
    else
        shape = ctx->shape_defaults.shape_type;

    switch (shape) {
    case 1:
        // Circle
        ctx->shape_defaults.x = x;
        ctx->shape_defaults.y = y;
        ctx->shape_defaults.radius = r;
        skiaShapes[CB_SHAPE_CIRCLE](ctx,&ctx->shape_defaults);
        break;

    case 2:
        // Rectangle
        ctx->shape_defaults.x =  x;
        ctx->shape_defaults.y = y;
        ctx->shape_defaults.width = ctx->shape_defaults.width? ctx->shape_defaults.width : 2*r;
        ctx->shape_defaults.height = ctx->shape_defaults.height? ctx->shape_defaults.height: 2*r;
        skiaShapes[CB_SHAPE_RECTANGLE](ctx, &ctx->shape_defaults);
        break;

    case 3:
        // Triangle
        skiaShapes[CB_SHAPE_TRIANGLE] (ctx,&ctx->shape_defaults);
        break;
    case 4:
        // Star
        skiaShapes[CB_SHAPE_STAR] (ctx,&ctx->shape_defaults);
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
            skiaRandomizePaintColor(ctx);
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
    ctx->skia_paint->setStrokeWidth(ctx->shape_defaults.stroke_width?ctx->shape_defaults.stroke_width:5);
    ctx->skia_paint->setStrokeJoin(ctx->shape_defaults.join_style?(SkPaint::Join)(ctx->shape_defaults.join_style % 5):SkPaint::kDefault_Join);
    ctx->skia_paint->setStrokeCap(ctx->shape_defaults.cap_style?(SkPaint::Cap)(ctx->shape_defaults.cap_style % 5):SkPaint::kDefault_Cap);
    fPE.reset(SkDashPathEffect::Create(vals, 4, 0));
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
    if(ctx->shape_defaults.animation)
    {
        int num_particles = ctx->shape_defaults.animation;
        particles = (kinetics_t *) malloc (sizeof (kinetics_t) * num_particles);
        int i,j ;
        for (i = 0; i < num_particles; i++)
            kinetics_init (&particles[i], ctx);

        for (j=0;j<num_particles;j++){
            ctx->skia_canvas->drawColor(SK_ColorWHITE);
            for (i = 0; i < num_particles; i++) {
                kinetics_update(&particles[i], 0.3);
                skiaRandomizePaintColor(ctx);
                //draw_stroke(ctx,ctx->skia_canvas,&particles[i]);
                drawShape(ctx,particles[i].x,particles[i].y,&particles[i]);
            }
        }
    }
    /* Static clip */
    else
        ctx->shape_defaults.multi_shapes?draw_stroke(ctx,ctx->skia_canvas,NULL):drawShape(ctx,ctx->shape_defaults.x?ctx->shape_defaults.x:100,ctx->shape_defaults.y?ctx->shape_defaults.y:100);
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
