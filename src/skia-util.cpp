/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <err.h>
#include <assert.h>

#include <SkBitmapDevice.h>

#include <cairo.h>

#include "caskbench.h"


void
write_image_file_skia (const char *fname, caskbench_context_t *context)
{
    SkBitmap bitmap;
    void * data;
    cairo_surface_t* surface;

    bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                     context->canvas_width, context->canvas_height);
    SkImageInfo info = SkImageInfo::Make(context->canvas_width, context->canvas_height,
                                         kBGRA_8888_SkColorType,
                                         kPremul_SkAlphaType);
    bitmap.allocPixels(info);
    context->skia_canvas->flush();
    if (!context->skia_canvas->readPixels(&bitmap, 0, 0)) {
        warnx("Could not read pixels from skia device\n");
        return;
    }

    data = bitmap.getPixels();
    assert(data);
    surface = cairo_image_surface_create_for_data((unsigned char*)data,
                                                  CAIRO_FORMAT_ARGB32,
                                                  context->canvas_width, context->canvas_height,
                                                  4 * context->canvas_width);

    cairo_status_t status = cairo_surface_status (surface);
    if (status != CAIRO_STATUS_SUCCESS) {
        warnx("Error writing skia surface to file: %s\n", cairo_status_to_string(status));
        return;
    }
    cairo_surface_write_to_png (surface, fname);
    cairo_surface_destroy(surface);
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
