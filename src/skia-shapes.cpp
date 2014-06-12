/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <SkCanvas.h>
#include <SkShader.h>
#include <SkImageDecoder.h>
#include <effects/SkGradientShader.h>
#include <SkDashPathEffect.h>

#include "forward.h"
#include "caskbench.h"
#include "caskbench_context.h"
#include "skia-shapes.h"

SkAutoTUnref<SkPathEffect> pE;

#ifndef SkToS32
int32_t SkToS32(intmax_t x) { return (int32_t)x; }
#endif

bool gPrintInstCount = false;


SkColor
skiaRandomColor()
{
    unsigned int red, blue, green, alpha;
    red = int( 255 * (double)rand()/RAND_MAX );
    green = int( 255 * (double)rand()/RAND_MAX );
    blue = int( 255 * (double)rand()/RAND_MAX );
    alpha = int( 255 * (double)rand()/RAND_MAX );
    return SkColorSetARGB(alpha, red, green, blue);
}

void
skiaRandomizePaintColor(caskbench_context_t *ctx)
{
    ctx->skia_paint->setColor(skiaRandomColor());
}

SkShader*
skiaCreateLinearGradientShader(int y1, int y2)
{
    SkColor linearColors[2];
    SkPoint linearPoints[2];

    linearColors[0] = skiaRandomColor();
    linearColors[1] = SkColorSetARGB (200,200,200,200);
    linearPoints[0].fX = SkIntToScalar(0);
    linearPoints[0].fY = SkIntToScalar(y1);
    linearPoints[1].fX = SkIntToScalar(0);
    linearPoints[1].fY = SkIntToScalar(y2);

    return SkGradientShader::CreateLinear(
        linearPoints, linearColors, NULL, 2,
        SkShader::kClamp_TileMode, NULL);
}

SkShader*
skiaCreateRadialGradientShader(int x, int y, int r)
{
    SkPoint center;
    SkColor linearColors[2];
    SkScalar radialPoints[4];

    linearColors[0] = SkColorSetARGB (200,200,200,200);
    linearColors[1] = skiaRandomColor();
    center.set(x+r, y+r);

    return SkGradientShader::CreateRadial(
        center, r,
        linearColors, NULL, 2,
        SkShader::kClamp_TileMode, NULL);
}

SkShader*
skiaCreateBitmapShader(const char *image_path)
{
    SkBitmap bm;

    SkImageDecoder::DecodeFile(image_path, &bm);
    return SkShader::CreateBitmapShader(bm, SkShader::kClamp_TileMode,
                                        SkShader::kClamp_TileMode);
}

void
skiaDrawLine(caskbench_context_t *ctx, shapes_t *args)
{
    SkPath path;

    path.moveTo(args->x, args->y);
    path.rLineTo(args->x + args->width, args->y + args->height);

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));
}

void
skiaDrawQuadraticCurve(caskbench_context_t *ctx, shapes_t *args)
{
    SkPath path;

    path.moveTo(args->x, args->y);
    path.rQuadTo(args->dx1, args->dy1,
                 args->width, args->height);

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));
}

void
skiaDrawCubicCurve(caskbench_context_t *ctx, shapes_t *args)
{
    SkPath path;

    path.moveTo(args->x, args->y);
    path.rCubicTo(args->dx1, args->dy1,
                  args->dx2, args->dy2,
                  args->width, args->height);

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));
}

void
skiaDrawCircle(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->skia_canvas->drawCircle(args->x + args->radius,
                                 args->y + args->radius,
                                 args->radius,
                                 *(ctx->skia_paint));
}

void
skiaDrawRectangle(caskbench_context_t *ctx, shapes_t *args)
{
    SkRect r;
    r.set(args->x, args->y,
          args->x + args->width, args->y + args->height);
    ctx->skia_canvas->drawRect(r, *(ctx->skia_paint));
}

void
skiaDrawTriangle(caskbench_context_t *ctx, shapes_t *args)
{
    SkPath path;

    // Temporarily disable anti-aliasing to work around crash in GlShader
    ctx->skia_paint->setAntiAlias(false);

    path.moveTo(args->x, args->y + 2*args->radius);
    path.rLineTo(2*args->radius, 0);
    path.rLineTo(-args->radius, -2*args->radius);
    path.close();

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));

    ctx->skia_paint->setAntiAlias(true);
}

void
skiaDrawStar(caskbench_context_t *ctx, shapes_t *args)
{
    int px = args->x + 2*args->radius * star_points[0][0]/200.0;
    int py = args->y + 2*args->radius * star_points[0][1]/200.0;
    SkPath path;
    path.moveTo(px, py);

    for (int p = 1; p < 10; p++ ) {
        px = args->x + 2*args->radius * star_points[p][0]/200.0;
        py = args->y + 2*args->radius * star_points[p][1]/200.0;
        path.lineTo(px, py);
    }
    path.close();

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));
}

void
skiaDrawRoundedRectangle (caskbench_context_t *ctx, shapes_t *args)
{
    SkRect rect;
    rect.set(args->x, args->y, args->x + args->width, args->y + args->height);
    ctx->skia_canvas->drawRoundRect(rect, 4.0, 4.0, *(ctx->skia_paint));
}

void (*skiaShapes[CB_SHAPE_END-1])(caskbench_context_t *ctx , shapes_t *args) = {
    skiaDrawCircle,
    skiaDrawRectangle,
    skiaDrawTriangle,
    skiaDrawStar,
    skiaDrawRoundedRectangle
};


void
skiaDrawRandomizedShape(caskbench_context_t *ctx, shapes_t *shape)
{
    // Shape Type
    if (shape->shape_type == CB_SHAPE_NONE)
        shape->shape_type = generate_random_shape();

    // Options for fill, gradient and transparency
    SkShader* shader = NULL;
    switch (shape->fill_type) {
        case CB_FILL_IMAGE_PATTERN:
            shader = skiaCreateBitmapShader(ctx->stock_image_path);
            break;
        case CB_FILL_RADIAL_GRADIENT:
            shader = skiaCreateRadialGradientShader(shape->x, shape->y, shape->radius);
            break;
        case CB_FILL_LINEAR_GRADIENT:
            shader = skiaCreateLinearGradientShader(shape->y, shape->y + shape->height);
            break;
        case CB_FILL_NONE:
        case CB_FILL_SOLID:
        default:
            SkColor color;
            if (shape->fill_color != -1) {
                color = SkColorSetARGB (shape->fill_color & 255,
                                       (shape->fill_color >> 24) & 255,
                                       (shape->fill_color >> 16) & 255,
                                       (shape->fill_color >> 8) & 255);
            }
            else {
                color = skiaRandomColor();
            }
            ctx->skia_paint->setColor(color);
            break;
    }
    if (shader)
        ctx->skia_paint->setShader (shader);

    // Draw
    ctx->skia_paint->setStyle(SkPaint::kFill_Style);
    skiaShapes[shape->shape_type-1] (ctx, shape);

    // Stroke styles
    if (shape->stroke_width)
    {
        ctx->skia_paint->setShader (NULL);
        ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
        ctx->skia_paint->setStrokeWidth(shape->stroke_width);
        ctx->skia_paint->setStrokeJoin((SkPaint::Join)shape->join_style);
        ctx->skia_paint->setStrokeCap((SkPaint::Cap)shape->cap_style);
        if (shape->dash_style == 0)
        {
            SkScalar vals[] = { SkIntToScalar(1), SkIntToScalar(1)  };
            ctx->skia_paint->setPathEffect(NULL);
            pE.reset(SkDashPathEffect::Create(vals, 2, 0));
            ctx->skia_paint->setPathEffect(SkDashPathEffect::Create(vals, 2, 0))->unref();
        }
        ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
        SkColor color;
        if (shape->stroke_color != -1) {
            color = SkColorSetRGB (shape->stroke_color >> 24 & 255,
                                  (shape->stroke_color >> 16) & 255,
                                  (shape->stroke_color >> 8) & 255);
        }
        else {
            color = skiaRandomColor();
        }
        ctx->skia_paint->setColor(color);
        skiaShapes[shape->shape_type-1] (ctx, shape);
    }

    // Cleanup
    ctx->skia_canvas->flush();
    ctx->skia_paint->setPathEffect(NULL);
    if (shader != NULL) {
        ctx->skia_paint->setShader (NULL);
        delete shader;
    }
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
