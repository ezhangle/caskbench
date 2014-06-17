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

#include "forward.h"
#include "image.h"

static image_state_t *state;
SkBitmap skia_bitmap;

static bool
convert_skbitmap_to_ximage(const SkBitmap& bitmap) {
    sk_bzero(&state->image, sizeof(state->image));

    int bitsPerPixel = bitmap.bytesPerPixel() * 8;
    state->image.width = bitmap.width();
    state->image.height = bitmap.height();
    state->image.format = ZPixmap;
    state->image.data = (char*) bitmap.getPixels();
    state->image.byte_order = LSBFirst;
    state->image.bitmap_unit = 4 * 8;
    state->image.bitmap_bit_order = LSBFirst;
    state->image.bitmap_pad = 4 * 8;
    state->image.depth = 32;
    state->image.bytes_per_line = bitmap.width() * 4;
    state->image.bits_per_pixel = 4 * 8;
    return XInitImage(&state->image);
}


SkBaseDevice *
create_skia_device_image (const device_config_t& config)
{
    state = (image_state_t*) malloc (sizeof (image_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!skia_bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                               config.width, config.height)) {
        warnx("Failed to configure bitmap\n");
        return NULL;
    }
    SkImageInfo info = SkImageInfo::Make(config.width, config.height,
                                         kBGRA_8888_SkColorType,
                                         kPremul_SkAlphaType);
    if (! skia_bitmap.allocPixels(info)) {
        warnx("Failed to allocate pixels\n");
        return NULL;
    }

    if (!createImageWindow(state, config)) {
        cleanup_state_image(state);
        return NULL;
    }

    return new SkBitmapDevice (skia_bitmap);
}

void
destroy_skia_image(void)
{
    destroyImageWindow(state);
    cleanup_state_image(state);
}

void
update_skia_image(void)
{
    if (convert_skbitmap_to_ximage (skia_bitmap)) {
        updateImageWindow(state);
    }
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
