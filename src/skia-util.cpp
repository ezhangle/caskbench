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
#include <SkImageEncoder.h>
#include <SkString.h>

#include "caskbench.h"


void
write_image_file_skia (const char *fname, caskbench_context_t *context)
{
    SkBitmap bitmap;
    void * data;
    SkString path(fname);
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
    SkImageEncoder::EncodeFile(path.c_str(), bitmap, SkImageEncoder::kPNG_Type, 0);
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
