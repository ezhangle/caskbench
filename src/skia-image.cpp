#include <err.h>
#include <stdlib.h>
#include <SkBitmapDevice.h>

#include "image.h"

static image_state_t *state;

SkBaseDevice *
create_skia_device_image (int width, int height)
{
    SkBitmap skia_bitmap;
    skia_bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                          width, height);
    skia_bitmap.allocPixels();
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
