/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <math.h>
#include <unistd.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkDraw.h>
#include <effects/SkGradientShader.h>
#include <SkGraphics.h>
#include <SkDashPathEffect.h>

#include "caskbench.h"
#include "skia-shapes.h"
#include "kinetics.h"

SkAutoTUnref<SkPathEffect> pE;
static kinetics_t *skia_particles;
static int element_spacing;
static int num_x_elements;
static int num_y_elements;

int
sk_setup_fill(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;

    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    return 1;
}

void
sk_teardown_fill(void)
{
}

void drawSkiaShapes(caskbench_context_t *ctx, kinetics_t *particle)
{
    int i, j, r, p;
    int old_x, old_y;
    shapes_t shape;

    shape_copy(&ctx->shape_defaults, &shape);

    shape.radius = 0.9 * element_spacing / 2;

    if (shape.shape_type != CB_SHAPE_NONE)
    {
        if (!(shape.x && shape.y))
        {
            shape.x = ctx->canvas_width/2;
            shape.y = ctx->canvas_height/2;
        }
        if (!shape.multi_shapes)
            if (!(shape.width && shape.height))
            {
                shape.width = 100;
                shape.height = 50;
            }
    } else if (!shape.multi_shapes) {
        shape.shape_type = (shape_type_t) (1 + (4.0 * rand())/RAND_MAX);
    }

    //Stroke styles
    if (shape.stroke_width)
    {
        ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
        ctx->skia_paint->setStrokeWidth(shape.stroke_width);
        ctx->skia_paint->setStrokeJoin((SkPaint::Join)shape.join_style);
        ctx->skia_paint->setStrokeCap((SkPaint::Cap)shape.cap_style);
        if (shape.dash_style == 0)
        {
            SkScalar vals[] = { SkIntToScalar(1), SkIntToScalar(1)  };
#if USE_LEGACY_SKIA_SRA
            pE.reset(new SkDashPathEffect(vals, 2, 0));
            ctx->skia_paint->setPathEffect(new SkDashPathEffect(vals, 2, 0));
#else
            pE.reset(SkDashPathEffect::Create(vals, 2, 0));
            ctx->skia_paint->setPathEffect(SkDashPathEffect::Create(vals, 2, 0));
#endif
            ctx->skia_paint->setPathEffect(pE);
        }
    }

    if (!shape.animation && !shape.multi_shapes)
    {
        num_x_elements = 1;
        num_y_elements = 1;

        shape.radius = 40;
    }

    if (!shape.width)
        shape.width = 2*shape.radius;

    if (!shape.height)
        shape.height = 2*shape.radius;

    bool randomize_color = true;
    if (shape.red > 0 || shape.blue > 0 || shape.green > 0 || shape.alpha > 0)
    {
        randomize_color = false;
        ctx->skia_paint->setARGB(255*((double)shape.alpha ? (double)shape.alpha:(double)1),
                                 255*((double)shape.red ? (double)shape.red:(double)0),
                                 255*((double)shape.green ? (double)shape.green:(double)0),
                                 255*(shape.blue ? (double)shape.blue:(double)0) );
    }

    for (j=0; j<num_y_elements; j++) {
        int y = particle? particle->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            int x = particle? particle->x : i * element_spacing;

            if (!shape.multi_shapes && !shape.animation) {
                x = shape.x;
                y = shape.y;
            } else {
                shape.x = x;
                shape.y = y;
            }

            // Options for fill, gradient and transparency
            SkShader* shader = NULL;
            if (shape.fill_type == CB_FILL_NONE)
            {
                if (randomize_color)
                    skiaRandomizePaintColor(ctx);
            }
            else if (shape.fill_type == CB_FILL_SOLID)
            {
                if (randomize_color)
                    skiaRandomizePaintColor(ctx);
            }
            else if (shape.fill_type == CB_FILL_IMAGE_PATTERN)
            {
                shader = skiaCreateBitmapShader(ctx->stock_image_path);
            }
            else if (shape.fill_type == CB_FILL_RADIAL_GRADIENT)
            {
                shader = skiaCreateRadialGradientShader(x, y, shape.radius);
            }
            else if (shape.fill_type == CB_FILL_LINEAR_GRADIENT)
            {
                shader = skiaCreateLinearGradientShader(y, y + shape.height);
            }

            if (shader)
                ctx->skia_paint->setShader (shader);

            skiaShapes[shape.shape_type] (ctx, &shape);

            ctx->skia_canvas->flush();
            if (shape.fill_type != CB_FILL_NONE)
                if (shader != NULL)
                {
                    ctx->skia_paint->setShader (NULL);
                    delete shader;
                }
            ctx->skia_paint->setPathEffect(NULL);
        }
    }
}

int
sk_test_fill(caskbench_context_t *ctx)
{
    for (int i = 0;i < ctx->size; i++)
    {
        // Animation / Kinematics of single or multi shape
        if (ctx->shape_defaults.animation)
        {
            int num_particles = ctx->shape_defaults.animation;
            double start_frame, stop_frame, delta;
            skia_particles = (kinetics_t *) malloc (sizeof (kinetics_t) * num_particles);
            int i,j ;
            for (i = 0; i < num_particles; i++)
                kinetics_init(&skia_particles[i]);
            delta = 0;

            for (j=0;j<num_particles;j++){
                start_frame = get_tick();
                ctx->skia_canvas->drawColor(SK_ColorBLACK);

                for (i = 0; i < num_particles; i++) {
                    kinetics_update(&skia_particles[i], 0.1);
                    drawSkiaShapes(ctx, &skia_particles[i]);
                }
                stop_frame = get_tick();
                delta = stop_frame - start_frame;
            }

        }

        // Drawing of multishape on a grid
        else
        {
            drawSkiaShapes(ctx, NULL);
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
