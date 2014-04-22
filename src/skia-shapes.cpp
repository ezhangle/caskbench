#include <config.h>

#include <SkCanvas.h>

#include "skia-shapes.h"

void skiaDrawCircle(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->skia_canvas->drawCircle(args->centre_x, args->centre_y, args->radius,
                                 *(ctx->skia_paint));
}

void skiaDrawRectangle(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->skia_canvas->drawRectCoords(args->centre_x, args->centre_y, args->width, args->height,
                                     *(ctx->skia_paint));
}

void skiaDrawTriangle(caskbench_context_t *ctx, shapes_t *args)
{
    for (int p = 0; p < ctx->shape_args.numpoints ; p++ ) {
        if(p == 0)
            args->path.moveTo(args->points[p][0], args->points[p][1]);
        else
            args->path.rLineTo(args->points[p][0], args->points[p][1]);
    }

    ctx->skia_canvas->drawPath(args->path,
                               *(ctx->skia_paint));
}

void skiaDrawStar(caskbench_context_t *ctx, shapes_t *args)
{
    for (int p = 0; p < args->numpoints; p++ ) {
        if(p == 0)
            args->path.moveTo(args->points[p][0], args->points[p][1]);
        else
            args->path.lineTo(args->points[p][0], args->points[p][1]);
    }
    ctx->skia_canvas->drawPath(args->path,
                               *(ctx->skia_paint));
}

void skiaDrawRoundedRectangle (caskbench_context_t *ctx, shapes_t *args)
{
    args->rect.set(args->centre_x, args->centre_y, args->width, args->height);

    ctx->skia_canvas->drawRoundRect(args->rect, 4.0, 4.0, *(ctx->skia_paint));

}

void (*skiaShapes[5])(caskbench_context_t *ctx , shapes_t *args) = {
    skiaDrawCircle,
    skiaDrawRectangle,
    skiaDrawTriangle,
    skiaDrawStar,
    skiaDrawRoundedRectangle
};

