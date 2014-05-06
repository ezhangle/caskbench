#include <config.h>

#include <err.h>
#include <assert.h>
#include <cairo.h>

#include "caskbench.h"

void
write_image_file_cairo (const char *fname, caskbench_context_t *context)
{
    assert (context->cairo_surface);
    cairo_status_t status = cairo_surface_status (context->cairo_surface);
    if (status != CAIRO_STATUS_SUCCESS) {
        warnx("Error writing cairo surface to file: %s\n", cairo_status_to_string(status));
        return;
    }
    cairo_surface_write_to_png (context->cairo_surface, fname);
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
