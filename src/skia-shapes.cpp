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

#include "skia-shapes.h"

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

void skiaDrawCircle(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->skia_canvas->drawCircle(args->x + args->radius,
                                 args->y + args->radius,
                                 args->radius,
                                 *(ctx->skia_paint));
}

void skiaDrawRectangle(caskbench_context_t *ctx, shapes_t *args)
{
    SkRect r;
    r.set(args->x, args->y,
          args->x + args->width, args->y + args->height);
    ctx->skia_canvas->drawRect(r, *(ctx->skia_paint));
}

void skiaDrawTriangle(caskbench_context_t *ctx, shapes_t *args)
{
    SkPath path;

    // Temporarily disable anti-aliasing to work around crash in GlShader
    ctx->skia_paint->setAntiAlias(false);

    path.moveTo(args->x, args->y + 2*args->radius);
    path.rLineTo(2*args->radius, 0);
    path.rLineTo(-args->radius, -2*args->radius);

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));

    ctx->skia_paint->setAntiAlias(true);
}

void skiaDrawStar(caskbench_context_t *ctx, shapes_t *args)
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

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));
}

void skiaDrawRoundedRectangle (caskbench_context_t *ctx, shapes_t *args)
{
    SkRect rect;
    rect.set(args->x, args->y, args->width, args->height);

    ctx->skia_canvas->drawRoundRect(rect, 4.0, 4.0, *(ctx->skia_paint));
}

void (*skiaShapes[5])(caskbench_context_t *ctx , shapes_t *args) = {
    skiaDrawCircle,
    skiaDrawRectangle,
    skiaDrawTriangle,
    skiaDrawStar,
    skiaDrawRoundedRectangle
};

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
