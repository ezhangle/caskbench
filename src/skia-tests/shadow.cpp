/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <SkCanvas.h>
#include <SkGradientShader.h>
#include <SkBlurMaskFilter.h>
#include <SkBlurMask.h>

#include "forward.h"
#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"
#include "kinetics.h"

static double line_width;

static double shadow_x_blur;
static double shadow_y_blur;

static double spread_line_width;
static double spread_x_blur;
static double spread_y_blur;

static double drop_x_blur;
static double drop_y_blur;
static double drop_x_offset;
static double drop_y_offset;
static double drop_line_width;

static double x_offset;
static double y_offset;

SkMaskFilter *mf,*mf_spread;
static kinetics_t *particles;

static void particle_init ()
{
    line_width = 20.0;

    /* Inset shadow */
    shadow_x_blur = 5;
    shadow_y_blur = 2;

    x_offset = 6;
    y_offset = 1;

    /* Spread */
    spread_x_blur = 5;
    spread_y_blur = 2;
    spread_line_width = 3;

    /* Drop shadow */
    drop_x_blur = 10;
    drop_y_blur = 8;
    drop_x_offset = -30;
    drop_y_offset = -5;
    drop_line_width = line_width;

    SkBlurMaskFilter::BlurFlags flags = SkBlurMaskFilter::kNone_BlurFlag;
    mf = SkBlurMaskFilter::Create(kInner_SkBlurStyle, (SkDoubleToScalar(drop_line_width)), flags);
    flags = SkBlurMaskFilter::kNone_BlurFlag;
    mf_spread = SkBlurMaskFilter::Create(kInner_SkBlurStyle, (SkDoubleToScalar(spread_line_width)), flags);
}

static void fill_particle (caskbench_context_t *ctx, shapes_t *shape)
{
    SkPaint *paint = ctx->skia_paint;
    SkCanvas *canvas = ctx->skia_canvas;
    /* Drop shadow */
    paint->setMaskFilter(mf);
    paint->setStrokeWidth(drop_line_width);
    paint->setARGB(255*0.5, 0, 0, 0);
    canvas->drawCircle(SkDoubleToScalar(shape->x+drop_x_offset), SkDoubleToScalar(shape->y+drop_y_offset), 40, *paint);
    paint->setMaskFilter(mf_spread);
    paint->setARGB(255, 0, 0, 0);
    canvas->drawCircle(SkDoubleToScalar(shape->x-shadow_x_blur+3), SkDoubleToScalar(shape->y-shadow_y_blur), 40, *paint);
    /* Ring */
    skiaRandomizePaintColor(ctx);
    paint->setStrokeWidth(line_width);
    canvas->drawCircle(SkDoubleToScalar(shape->x), SkDoubleToScalar(shape->y), 40, *paint);

    /* Spread */
    paint->setMaskFilter(mf_spread);
    paint->setStrokeWidth(spread_line_width);
    paint->setARGB(255, 255, 255, 255);
    canvas->drawCircle(SkDoubleToScalar(shape->x+spread_x_blur), SkDoubleToScalar(shape->y+spread_y_blur), 40, *paint);
}

int
sk_setup_shadow(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;
    /* Animation & Shadow setup */
    particles = (kinetics_t *) malloc (sizeof (kinetics_t) * ctx->size);
    for (int i = 0; i < ctx->size; i++)
        kinetics_init(&particles[i], ctx);

    particle_init ();
    /* Set Style and Stroke Width */
    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
    ctx->skia_paint->setStrokeWidth(20);

    return 1;
}

void
sk_teardown_shadow(void)
{
    free(particles);
}

int
sk_test_shadow(caskbench_context_t *ctx)
{
    ctx->skia_canvas->drawColor(SK_ColorWHITE);

    for (int i = 0; i < ctx->size; i++) {
        shapes_t shape;
        kinetics_t *particle = &particles[i];
        kinetics_update(particle, 0.1);

        shape_copy(&ctx->shape_defaults, &shape);
        shape.width = particle->width;
        shape.height = particle->height;
        shape.x = particle->x;
        shape.y = particle->y;
        if (!shape.radius)
            shape.radius = 40;

        fill_particle (ctx, &shape);
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
