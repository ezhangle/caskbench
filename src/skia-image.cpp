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
