#include <config.h>

#include <SkCanvas.h>

#include "skia-shapes.h"
static int star_points[11][2] = {
    { 0, 85 },
    { 75, 75 },
    { 100, 10 },
    { 125, 75 },
    { 200, 85 },
    { 150, 125 },
    { 160, 190 },
    { 100, 150 },
    { 40, 190 },
    { 50, 125 },
    { 0, 85 }
};
void
skiaRandomizeColor(caskbench_context_t *ctx)
{
    unsigned int red, blue, green, alpha;
    red = int( 255 * (double)rand()/RAND_MAX );
    green = int( 255 * (double)rand()/RAND_MAX );
    blue = int( 255 * (double)rand()/RAND_MAX );
    alpha = int( 255 * (double)rand()/RAND_MAX );
    ctx->skia_paint->setARGB(alpha,red, green, blue );
}

void skiaDrawCircle(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->skia_canvas->drawCircle(args->center_x, args->center_y, args->radius,
                                 *(ctx->skia_paint));
}

void skiaDrawRectangle(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->skia_canvas->drawRectCoords(args->center_x, args->center_y, args->width, args->height,
                                     *(ctx->skia_paint));
}

void skiaDrawTriangle(caskbench_context_t *ctx, shapes_t *args)
{
    SkPath path;
    path.moveTo(args->center_x, args->center_y+2*args->radius);
    path.rLineTo(2*args->radius, 0);
    path.rLineTo(-args->radius, -2*args->radius);

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));
}

void skiaDrawStar(caskbench_context_t *ctx, shapes_t *args)
{
    int px = args->center_x + 2*args->radius * star_points[0][0]/200.0;
    int py = args->center_y + 2*args->radius * star_points[0][1]/200.0;
    SkPath path;
    path.moveTo(px, py);

    for (int p = 1; p < 10; p++ ) {
        px = args->center_x + 2*args->radius * star_points[p][0]/200.0;
        py = args->center_y + 2*args->radius * star_points[p][1]/200.0;
        path.lineTo(px, py);
    }

    ctx->skia_canvas->drawPath(path, *(ctx->skia_paint));
}

void skiaDrawRoundedRectangle (caskbench_context_t *ctx, shapes_t *args)
{
    args->rect.set(args->center_x, args->center_y, args->width, args->height);

    ctx->skia_canvas->drawRoundRect(args->rect, 4.0, 4.0, *(ctx->skia_paint));
}

void (*skiaShapes[5])(caskbench_context_t *ctx , shapes_t *args) = {
    skiaDrawCircle,
    skiaDrawRectangle,
    skiaDrawTriangle,
    skiaDrawStar,
    skiaDrawRoundedRectangle
};

