#include <SkCanvas.h>
#include <SkPaint.h>
#include <cairo.h>

#include "caskbench.h"

static int element_spacing;
static int num_x_elements;
static int num_y_elements;

int
sk_setup_multishape(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;

    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    // TODO: Create 10 different paint objects in a list to select between

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
    int i, j, x, y, r;

    r = 0.9 * element_spacing / 2;
    for (j=0; j<num_y_elements; j++) {
        y = j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            x = i * element_spacing;

            // TODO: Select a pre-defined paint object at random
            red = int( 255 * (double)rand()/RAND_MAX );
            green = int( 255 * (double)rand()/RAND_MAX );
            blue = int( 255 * (double)rand()/RAND_MAX );
            alpha = int( 255 * (double)rand()/RAND_MAX );
            ctx->skia_paint->setARGB(alpha, red, green, blue);

            // TODO: Randomly select different shapes to be drawn
            ctx->skia_canvas->drawCircle(x, y, r, *ctx->skia_paint);
        }
    }
    return 1;
}
