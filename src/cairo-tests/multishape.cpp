#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>

#include "caskbench.h"
#include "cairo-shapes.h"

static int element_spacing;
static int num_x_elements;
static int num_y_elements;

int
ca_setup_multishape(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;

    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    return 1;
}

void
ca_teardown_multishape(void)
{
}

int
ca_test_multishape(caskbench_context_t *ctx)
{
    int i, j, x, y, r, p, shape;
    cairo_t *cr = ctx->cairo_cr;

    r = 0.9 * element_spacing / 2;
    for (j=0; j<num_y_elements; j++) {
        y = j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            x = i * element_spacing;

            cairoRandomizeColor(ctx);

            shape = (4.0 * rand())/RAND_MAX;
            switch (shape) {
            case 0:
                // Circle
                cairo_arc (cr, x+r, y+r, r, 0, 2*M_PI);
                cairo_fill (cr);
                break;

            case 1:
                // Rectangle
                cairo_rectangle (cr, x, y, 2*r, 2*r);
                cairo_fill (cr);
                break;

            case 2:
                // Triangle
                cairo_move_to (cr, x, y+2*r);
                cairo_rel_line_to (cr, 2*r, 0);
                cairo_rel_line_to (cr, -r, -2*r);
                cairo_fill (cr);
                break;

            case 3:
                // Star
                cairo_move_to (cr, x, y);
                for (p = 0; p < 10; p++ ) {
                    int px = x + 2*r * star_points[p][0]/200.0;
                    int py = y + 2*r * star_points[p][1]/200.0;
                    if (p == 0)
                        cairo_move_to (cr, px, py);
                    else
                        cairo_line_to(cr, px, py);
                }
                cairo_close_path(cr);
                break;

            default:
                break;
            }
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
