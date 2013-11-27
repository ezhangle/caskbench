#include <err.h>
#include <stdlib.h>
#include <SkBitmapDevice.h>

SkBaseDevice *
create_skia_device_image (int width, int height)
{
  SkBitmap skia_bitmap;
  skia_bitmap.setConfig(SkBitmap::kARGB_8888_Config,
			width, height);
  skia_bitmap.allocPixels();
  return new SkBitmapDevice (skia_bitmap);
}
