#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <math.h>

#include "caskbench.h"

static int element_spacing;
static int num_x_elements;
static int num_y_elements;
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

int
sk_setup_multishape(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;

    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    return 1;
}

void
sk_teardown_multishape(void)
{
}

int
sk_test_multishape(caskbench_context_t *ctx)
{
    unsigned char red, green, blue, alpha;
    int i, j, x, y, r, p, shape;

    r = 0.9 * element_spacing / 2;
    for (j=0; j<num_y_elements; j++) {
        y = j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            SkPath path;
            x = i * element_spacing;

            // TODO: Select a pre-defined paint object at random
            red = int( 255 * (double)rand()/RAND_MAX );
            green = int( 255 * (double)rand()/RAND_MAX );
            blue = int( 255 * (double)rand()/RAND_MAX );
            alpha = int( 255 * (double)rand()/RAND_MAX );
            ctx->skia_paint->setARGB(alpha, red, green, blue);

            shape = (4.0 * rand())/RAND_MAX;
            switch (shape) {
            case 0:
                // Circle
                ctx->skia_canvas->drawCircle(x+r, y+r, r,
                                             *(ctx->skia_paint));
                break;

            case 1:
                // Rectangle
                ctx->skia_canvas->drawRectCoords(x, y, x+2*r, y+2*r,
                                                 *(ctx->skia_paint));
                break;

            case 2:
                // Triangle
                path.moveTo(x, y+2*r);
                path.rLineTo(2*r, 0);
                path.rLineTo(-r, -2*r);
                ctx->skia_canvas->drawPath(path,
                                           *(ctx->skia_paint));
                break;

            case 3:
                // Star
                path.moveTo(x, y);
                for (p = 0; p < 10; p++ ) {
                    int px = x + 2*r * star_points[p][0]/200.0;
                    int py = y + 2*r * star_points[p][1]/200.0;
                    if (p == 0)
                        path.moveTo(px, py);
                    else
                        path.lineTo(px, py);
                }
                ctx->skia_canvas->drawPath(path,
                                           *(ctx->skia_paint));
                break;

            default:
                break;
            }
            ctx->skia_canvas->flush();
        }
    }
    return 1;
}
