/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <err.h>
#include <stdlib.h>

#include <SkBitmapDevice.h>

#include "image.h"
#include "device_config.h"

static image_state_t *state;

SkBaseDevice *
create_skia_device_image (const device_config_t& config)
{
    SkBitmap skia_bitmap;
    if (!skia_bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                               config.width, config.height)) {
        warnx("Failed to configure bitmap\n");
        return NULL;
    }
#ifdef USE_LEGACY_SKIA_SRA
    skia_bitmap.allocPixels();
#else
    SkImageInfo info = SkImageInfo::Make(config.width, config.height,
                                         kBGRA_8888_SkColorType,
                                         kPremul_SkAlphaType);
    if (! skia_bitmap.allocPixels(info)) {
        warnx("Failed to allocate pixels\n");
        return NULL;
    }
#endif
    return new SkBitmapDevice (skia_bitmap);
}

void
destroy_skia_image(void)
{
    cleanup_state_image(state);
}

void
update_skia_image(void)
{
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
