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

static void
_draw_shape(caskbench_context_t *ctx, shapes_t *shape)
{
    bool randomize_color = true;
    if (shape->red > 0 || shape->blue > 0 || shape->green > 0 || shape->alpha > 0)
    {
        randomize_color = false;
        ctx->skia_paint->setARGB(255.0*(shape->alpha ? shape->alpha : 1.0),
                                 255.0*(shape->red ? shape->red : 0.0),
                                 255.0*(shape->green ? shape->green : 0.0),
                                 255.0*(shape->blue ? shape->blue : 0.0) );
    }

    // Stroke styles
    if (shape->stroke_width)
    {
        ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
        ctx->skia_paint->setStrokeWidth(shape->stroke_width);
        ctx->skia_paint->setStrokeJoin((SkPaint::Join)shape->join_style);
        ctx->skia_paint->setStrokeCap((SkPaint::Cap)shape->cap_style);
        if (shape->dash_style == 0)
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

    // Options for fill, gradient and transparency
    SkShader* shader = NULL;
    if (shape->fill_type == CB_FILL_NONE ||
        shape->fill_type == CB_FILL_SOLID)
        if (randomize_color)
            ctx->skia_paint->setColor(skiaRandomColor());
        else if (shape->fill_type == CB_FILL_IMAGE_PATTERN)
            shader = skiaCreateBitmapShader(ctx->stock_image_path);
        else if (shape->fill_type == CB_FILL_RADIAL_GRADIENT)
            shader = skiaCreateRadialGradientShader(shape->x, shape->y, shape->radius);
        else if (shape->fill_type == CB_FILL_LINEAR_GRADIENT)
            shader = skiaCreateLinearGradientShader(shape->y, shape->y + shape->height);

    if (shader)
        ctx->skia_paint->setShader (shader);

    skiaShapes[shape->shape_type] (ctx, shape);

    ctx->skia_canvas->flush();
    ctx->skia_paint->setPathEffect(NULL);
    if (shader != NULL) {
        ctx->skia_paint->setShader (NULL);
        delete shader;
    }
}

int
sk_test_fill(caskbench_context_t *ctx)
{
    for (int i = 0;i < ctx->size; i++)
    {
        // Animation / Kinematics of single or multi shape
        if (ctx->shape_defaults.animation) {
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
                    shapes_t shape;
                    kinetics_t *particle = &skia_particles[i];

                    kinetics_update(particle, 0.1);
                    shape_copy(&ctx->shape_defaults, &shape);
                    if (shape.shape_type == CB_SHAPE_NONE)
                        shape.shape_type = (shape_type_t) (1 + (4.0 * rand())/RAND_MAX);

                    shape.width = particle->width;
                    shape.height = particle->height;
                    shape.x = particle->x;
                    shape.y = particle->y;
                    _draw_shape(ctx, &shape);
                }
                stop_frame = get_tick();
                delta = stop_frame - start_frame;
            }
        }

        // Drawing of multishape on a grid
        else if (ctx->shape_defaults.multi_shapes) {
            shapes_t shape;
            shape_copy(&ctx->shape_defaults, &shape);
            if (shape.shape_type == CB_SHAPE_NONE)
                shape.shape_type = (shape_type_t) (1 + (4.0 * rand())/RAND_MAX);

            shape.radius = 0.9 * element_spacing / 2;

            if (!shape.width)
                shape.width = 2*shape.radius;

            if (!shape.height)
                shape.height = 2*shape.radius;

            for (int j=0; j<num_y_elements; j++) {
                for (int i=0; i<num_x_elements; i++) {
                    shape.x = i * element_spacing;
                    shape.y = j * element_spacing;
                    _draw_shape(ctx, &shape);
                }
            }
        }
        else {
            shapes_t shape;
            shape_copy(&ctx->shape_defaults, &shape);
            if (shape.shape_type == CB_SHAPE_NONE)
                shape.shape_type = (shape_type_t) (1 + (4.0 * rand())/RAND_MAX);

            if (!(shape.x && shape.y))
            {
                shape.x = ctx->canvas_width/2;
                shape.y = ctx->canvas_height/2;
            }
            if (!(shape.width && shape.height))
            {
                shape.width = 100;
                shape.height = 50;
            }
            if (!shape.radius)
                shape.radius = 40;

            _draw_shape(ctx, &shape);
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
