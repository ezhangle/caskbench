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
#include <effects/SkGradientShader.h>
#include <SkDraw.h>
#include <effects/SkGradientShader.h>
#include <SkGraphics.h>
#include <SkImageDecoder.h>
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

    r = 0.9 * element_spacing / 2;

    shape_type_t shape_type = (shape_type_t) ctx->shape_defaults.shape_id;
    if (shape_type != CB_SHAPE_NONE)
    {
        if (!(ctx->shape_defaults.x && ctx->shape_defaults.y))
        {
            ctx->shape_defaults.x = ctx->canvas_width/2;
            ctx->shape_defaults.y = ctx->canvas_height/2;
        }
        if (!ctx->shape_defaults.multi_shapes)
            if (!(ctx->shape_defaults.width && ctx->shape_defaults.height))
            {
                ctx->shape_defaults.width = 100;
                ctx->shape_defaults.height = 50;
            }
        ctx->shape_defaults.radius = r;
    } else if (!ctx->shape_defaults.multi_shapes) {
        shape_type = (shape_type_t) (1 + (4.0 * rand())/RAND_MAX);
    }

    //Stroke styles
    if (ctx->shape_defaults.stroke_width)
    {
        ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
        ctx->skia_paint->setStrokeWidth(ctx->shape_defaults.stroke_width);
        ctx->skia_paint->setStrokeJoin((SkPaint::Join)ctx->shape_defaults.join_style);
        ctx->skia_paint->setStrokeCap((SkPaint::Cap)ctx->shape_defaults.cap_style);
        if (ctx->shape_defaults.dash_style == 0)
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

    if (!ctx->shape_defaults.animation && !ctx->shape_defaults.multi_shapes)
    {
        num_x_elements = 1;
        num_y_elements = 1;

        r = 40;
        old_x = ctx->shape_defaults.x;
        old_y = ctx->shape_defaults.y;
    }

    if (!ctx->shape_defaults.width)
        ctx->shape_defaults.width = 2*r;

    if (!ctx->shape_defaults.height)
        ctx->shape_defaults.height = 2*r;

    bool randomize_color = true;
    if (ctx->shape_defaults.red > 0 || ctx->shape_defaults.blue > 0 || ctx->shape_defaults.green > 0 || ctx->shape_defaults.alpha > 0)
    {
        randomize_color = false;
        ctx->skia_paint->setARGB(255*((double)ctx->shape_defaults.alpha ? (double)ctx->shape_defaults.alpha:(double)1),
                                 255*((double)ctx->shape_defaults.red ? (double)ctx->shape_defaults.red:(double)0),
                                 255*((double)ctx->shape_defaults.green ? (double)ctx->shape_defaults.green:(double)0),
                                 255*(ctx->shape_defaults.blue ? (double)ctx->shape_defaults.blue:(double)0) );
    }

    for (j=0; j<num_y_elements; j++) {
        int y = particle? particle->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            int x = particle? particle->x : i * element_spacing;

            ctx->shape_defaults.radius = r;
            if (!ctx->shape_defaults.multi_shapes && !ctx->shape_defaults.animation) {
                x = ctx->shape_defaults.x;
                y = ctx->shape_defaults.y;
            } else {
                ctx->shape_defaults.x = x;
                ctx->shape_defaults.y = y;
            }

            // Options for fill, gradient and transparency
            SkShader* shader = NULL;
            if (ctx->shape_defaults.fill_type == CB_FILL_NONE)
            {
                if (randomize_color)
                    skiaRandomizePaintColor(ctx);
            }
            else if (ctx->shape_defaults.fill_type == CB_FILL_SOLID)
            {
                if (randomize_color)
                    skiaRandomizePaintColor(ctx);
            }
            else if (ctx->shape_defaults.fill_type == CB_FILL_IMAGE_PATTERN)
            {
                SkBitmap    bm;

                SkImageDecoder::DecodeFile(ctx->shape_defaults.image_path, &bm);
                shader = SkShader::CreateBitmapShader(bm, SkShader::kClamp_TileMode,
                                        SkShader::kClamp_TileMode);
            }
            else if (ctx->shape_defaults.fill_type == CB_FILL_RADIAL_GRADIENT)
            {
                SkPoint center;
                SkColor linearColors[2];
                SkScalar radialPoints[4];

                linearColors[0] = SkColorSetARGB (200,200,200,200);
                linearColors[1] = skiaRandomColor();
                center.set(x+r, y+r);

                shader = SkGradientShader::CreateRadial(
                    center, r,
                    linearColors, NULL, 2,
                    SkShader::kClamp_TileMode, NULL);
            }
            else if (ctx->shape_defaults.fill_type == CB_FILL_LINEAR_GRADIENT)
            {
                SkColor linearColors[2];
                SkPoint linearPoints[2];

                linearColors[0] = skiaRandomColor();
                linearColors[1] = SkColorSetARGB (200,200,200,200);
                linearPoints[0].fX = SkIntToScalar(0);
                linearPoints[0].fY = SkIntToScalar(y);
                linearPoints[1].fX = SkIntToScalar(0);
                linearPoints[1].fY = SkIntToScalar(y + ctx->shape_defaults.height);

                shader = SkGradientShader::CreateLinear(
                    linearPoints, linearColors, NULL, 2,
                    SkShader::kClamp_TileMode, NULL);
            }

            if (shader)
                ctx->skia_paint->setShader (shader);

            skiaShapes[shape_type] (ctx, &ctx->shape_defaults);

            ctx->skia_canvas->flush();
            if (ctx->shape_defaults.fill_type != CB_FILL_NONE)
                if (shader != NULL)
                {
                    ctx->skia_paint->setShader (NULL);
                    delete shader;
                }
            ctx->skia_paint->setPathEffect(NULL);
        }
    }

    if (!ctx->shape_defaults.animation && !ctx->shape_defaults.multi_shapes)
    {
        ctx->shape_defaults.x = old_x;
        ctx->shape_defaults.y = old_y;
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
