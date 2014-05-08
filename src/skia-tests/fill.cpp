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
    int i, j, r, p, shape;
    r = 0.9 * element_spacing /2;
    int old_x, old_y;

    shape = ctx->shape_args.shape_id;
    if (shape)
    {
        if (!(ctx->shape_args.x && ctx->shape_args.y))
        {
            ctx->shape_args.x = ctx->canvas_width/2;
            ctx->shape_args.y = ctx->canvas_height/2;
        }
        if (!ctx->shape_args.multi_shapes)
            if (!(ctx->shape_args.width && ctx->shape_args.height))
            {
                ctx->shape_args.width = 100;
                ctx->shape_args.height = 50;
            }
        ctx->shape_args.radius = r;
    } else {
        shape = ((4.0 * rand())/RAND_MAX) + 1;
        if (!ctx->shape_args.multi_shapes)
            shape = 2;
    }

    //Stroke styles
    if (ctx->shape_args.stroke_width)
    {
        ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
        ctx->skia_paint->setStrokeWidth(ctx->shape_args.stroke_width);
        ctx->skia_paint->setStrokeJoin((SkPaint::Join)ctx->shape_args.join_style);
        ctx->skia_paint->setStrokeCap((SkPaint::Cap)ctx->shape_args.cap_style);
        if (ctx->shape_args.dash_style == 0)
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

    if (!ctx->shape_args.animation && !ctx->shape_args.multi_shapes)
    {
        num_x_elements = 1;
        num_y_elements = 1;

        r = 40;
        old_x = ctx->shape_args.x;
        old_y = ctx->shape_args.y;
    }

    bool randomize_color = true;
    if (ctx->shape_args.red > 0 || ctx->shape_args.blue > 0 || ctx->shape_args.green > 0 || ctx->shape_args.alpha > 0)
    {
        randomize_color = false;
        ctx->skia_paint->setARGB(255*((double)ctx->shape_args.alpha ? (double)ctx->shape_args.alpha:(double)1),
                                 255*((double)ctx->shape_args.red ? (double)ctx->shape_args.red:(double)0),
                                 255*((double)ctx->shape_args.green ? (double)ctx->shape_args.green:(double)0),
                                 255*(ctx->shape_args.blue ? (double)ctx->shape_args.blue:(double)0) );
    }

    for (j=0; j<num_y_elements; j++) {
        int y = particle? particle->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            int x = particle? particle->x : i * element_spacing;

            double y1, y2, cx, cy, rr;
            shape_type_t shape_type;
            switch (shape) {
            case 1:
                // Circle
                shape_type = CB_SHAPE_CIRCLE;
                ctx->shape_args.x = x;
                ctx->shape_args.y = y;
                ctx->shape_args.radius = r;

                y1 = y;
                y2 = y + 2*r;
                cx = x + r;
                cy = y + r;
                rr = r;

                break;

            case 2:
                // Rectangle
                shape_type = CB_SHAPE_RECTANGLE;
                ctx->shape_args.x = x;
                ctx->shape_args.y = y;

                if (!ctx->shape_args.width)
                    ctx->shape_args.width = 2*r;

                if (!ctx->shape_args.height)
                    ctx->shape_args.height = 2*r;

                y1 = y;
                y2 = y + 2*r;
                cx = x + r;
                cy = y + r;
                rr = r;

                break;

            case 3:
                // Triangle
                shape_type = CB_SHAPE_TRIANGLE;

                y1 = y;
                y2 = y + 2*r;
                cx = x;
                cy = y + 2*r;
                rr = r;

                break;

            case 4:
                // Star
                shape_type = CB_SHAPE_STAR;

                y1 = y;
                y2 = y + 2*r;
                cx = x;
                cy = y + 2*r;
                rr = r;

                break;

            default:
                // TODO: Should never reach this point
                break;
            }

            if (!ctx->shape_args.multi_shapes && !ctx->shape_args.animation) {
                x = ctx->shape_args.x;
                y = ctx->shape_args.y;
            }

            // Options for fill, gradient and transparency
            SkShader* shader = NULL;
            if (ctx->shape_args.fill_type == NULL)
            {
                if (randomize_color)
                    skiaRandomizePaintColor(ctx);
            }
            else if ((strcmp(ctx->shape_args.fill_type,"solid")) == 0)
            {
                if (randomize_color)
                    skiaRandomizePaintColor(ctx);
            }
            else if ((strcmp(ctx->shape_args.fill_type,"image-pattern")) == 0 &&(ctx->shape_args.image_path))
            {
                SkBitmap    bm;

                SkImageDecoder::DecodeFile(ctx->shape_args.image_path, &bm);
                shader = SkShader::CreateBitmapShader(bm, SkShader::kClamp_TileMode,
                                        SkShader::kClamp_TileMode);
            }
            else if ((strcmp(ctx->shape_args.fill_type,"radial-gradient")) == 0)
            {
                SkPoint center;
                SkColor linearColors[2];
                SkScalar radialPoints[4];

                linearColors[0] = SkColorSetARGB (200,200,200,200);
                linearColors[1] = skiaRandomColor();
                center.set(cx, cy);

                shader = SkGradientShader::CreateRadial(
                    center, rr,
                    linearColors, NULL, 2,
                    SkShader::kClamp_TileMode, NULL);
            }
            else if ((strcmp(ctx->shape_args.fill_type,"linear-gradient")) == 0)
            {
                SkColor linearColors[2];
                SkPoint linearPoints[2];

                linearColors[0] = skiaRandomColor();
                linearColors[1] = SkColorSetARGB (200,200,200,200);
                linearPoints[0].fX = SkIntToScalar(0);
                linearPoints[0].fY = SkIntToScalar(y1);
                linearPoints[1].fX = SkIntToScalar(0);
                linearPoints[1].fY = SkIntToScalar(y2);

                shader = SkGradientShader::CreateLinear(
                    linearPoints, linearColors, NULL, 2,
                    SkShader::kClamp_TileMode, NULL);
            }

            if (shader)
                ctx->skia_paint->setShader (shader);

            skiaShapes[shape_type] (ctx, &ctx->shape_args);

            ctx->skia_canvas->flush();
            if (ctx->shape_args.fill_type != NULL)
                if (shader != NULL)
                {
                    ctx->skia_paint->setShader (NULL);
                    delete shader;
                }
            ctx->skia_paint->setPathEffect(NULL);
        }
    }

    if (!ctx->shape_args.animation && !ctx->shape_args.multi_shapes)
    {
        ctx->shape_args.x = old_x;
        ctx->shape_args.y = old_y;
    }
}

int
sk_test_fill(caskbench_context_t *ctx)
{
    for (int i = 0;i < ctx->size; i++)
    {
        // Animation / Kinematics of single or multi shape
        if (ctx->shape_args.animation)
        {
            int num_particles = ctx->shape_args.animation;
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
