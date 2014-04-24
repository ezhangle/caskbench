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
    ctx->shape_args.numpoints = 3;
    ctx->shape_args.points = (double (*)[2]) malloc(ctx->shape_args.numpoints*2*(sizeof(double)));
    ctx->shape_args.points[0][0] = ctx->shape_args.centre_x;
    ctx->shape_args.points[0][1] = ctx->shape_args.centre_y+2*ctx->shape_args.radius;
    ctx->shape_args.points[1][0] = 2*ctx->shape_args.radius;
    ctx->shape_args.points[1][1] = 0;
    ctx->shape_args.points[2][0] = -ctx->shape_args.radius;
    ctx->shape_args.points[2][1] = -2*ctx->shape_args.radius;


    for (int p = 0; p < ctx->shape_args.numpoints ; p++ ) {
        if(p == 0)
            args->path.moveTo(args->points[p][0], args->points[p][1]);
        else
            args->path.rLineTo(args->points[p][0], args->points[p][1]);
    }

    ctx->skia_canvas->drawPath(args->path,
                               *(ctx->skia_paint));
    free (ctx->shape_args.points);

}

void skiaDrawStar(caskbench_context_t *ctx, shapes_t *args)
{
    ctx->shape_args.numpoints = 10;
    ctx->shape_args.points = (double (*)[2]) malloc(ctx->shape_args.numpoints*2*(sizeof(double)));

    for (int p = 0; p < 10; p++ ) {
        int px = ctx->shape_args.centre_x + 2*ctx->shape_args.radius * star_points[p][0]/200.0;
        int py = ctx->shape_args.centre_y + 2*ctx->shape_args.radius * star_points[p][1]/200.0;
        ctx->shape_args.points[p][0] = px;
        ctx->shape_args.points[p][1] = py;
    }

      for (int p = 0; p < args->numpoints; p++ ) {
        if(p == 0)
            args->path.moveTo(args->points[p][0], args->points[p][1]);
        else
            args->path.lineTo(args->points[p][0], args->points[p][1]);
    }
    ctx->skia_canvas->drawPath(args->path,
                               *(ctx->skia_paint));
    free (ctx->shape_args.points);
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

