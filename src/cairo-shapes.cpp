#include "cairo-shapes.h"




void cairoDrawCircle(caskbench_context_t *ctx, shapes_t *args)
{
	cairo_arc (ctx->cairo_cr, args->centre_x, args->centre_y, args->radius, 0, 2*M_PI);
}

void cairoDrawRectangle(caskbench_context_t *ctx, shapes_t *args)
{
	cairo_rectangle (ctx->cairo_cr, args->centre_x, args->centre_y, args->width, args->height);
}

void cairoDrawTriangle(caskbench_context_t *ctx, shapes_t *args)
{
	for (int p = 0; p < args->numpoints; p++ ) {
	if(p == 0)
		cairo_move_to (ctx->cairo_cr, args->points[p][0], args->points[0][1]);
	else
		cairo_rel_line_to (ctx->cairo_cr, args->points[p][0], args->points[p][1]);
	}

}

void cairoDrawStar(caskbench_context_t *ctx, shapes_t *args)
{
    for (int p = 0; p < args->numpoints; p++ ) {
    if(p == 0)
        cairo_move_to (ctx->cairo_cr, args->points[p][0], args->points[0][1]);
    else
        cairo_line_to (ctx->cairo_cr, args->points[p][0], args->points[p][1]);
    }
    cairo_close_path(ctx->cairo_cr);
}


void
cairoDrawRoundedRectangle (caskbench_context_t *ctx, shapes_t *args)
{
    cairo_move_to (ctx->cairo_cr, args->centre_x+args->radius, args->centre_y);
    cairo_arc (ctx->cairo_cr, args->centre_x+args->width-args->radius, args->centre_y+args->radius,   args->radius, M_PI + M_PI / 2, M_PI * 2        );
    cairo_arc (ctx->cairo_cr, args->centre_x+args->width-args->radius, args->centre_y+args->height-args->radius, args->radius, 0,               M_PI / 2        );
    cairo_arc (ctx->cairo_cr, args->centre_x+args->radius,   args->centre_y+args->height-args->radius, args->radius, M_PI/2,          M_PI            );
    cairo_arc (ctx->cairo_cr, args->centre_x+args->radius,   args->centre_y+args->radius,   args->radius, M_PI,            270 * M_PI / 180);
}


void (*cairoShapes[5])(caskbench_context_t *ctx , shapes_t *args) = {
															cairoDrawCircle,
															cairoDrawRectangle,
															cairoDrawTriangle,
															cairoDrawStar,
															cairoDrawRoundedRectangle 
															};

