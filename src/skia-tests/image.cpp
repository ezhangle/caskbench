// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include "err.h"
#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include "SkImageDecoder.h"
#include <cairo.h>

#include "SkStream.h"
#include "SkPixelRef.h"
 
#include "caskbench.h"

static SkBitmap bitmap;

int32_t SkToS32(intmax_t x) { return (int32_t)x; }

int
sk_setup_image(SkPaint *paint)
{
  const char* path = "/home/bryce/src/Caskbench/caskbench/data/romedalen.png";

  SkAutoTUnref<SkStreamRewindable> stream(SkStream::NewFromFile(path));
  if (!stream.get()) {
    warnx("Failed to create stream from %s\n", path);
    return 0;
  }
  if (!SkImageDecoder::DecodeStream(stream, &bitmap,
				    SkBitmap::kNo_Config,
				    SkImageDecoder::kDecodePixels_Mode,
				    NULL)) {
    warnx("Failed to decode stream\n");
    return 0;
  }
  bitmap.pixelRef()->setURI(path);

  //bool ret = SkImageDecoder::DecodeFile(path, &bitmap);
  bitmap.setImmutable();
  printf("bitmap: %d, %d\n", bitmap.width(), bitmap.height());
  bitmap.lockPixels();
  return 1;
}

int
sk_test_image(SkCanvas *canvas, SkPaint *paint)
{
  int i;
  for (i=0; i<32; i++) {
    canvas->drawBitmap(bitmap, 0, 0);
  }

  return 1;
}

void
sk_teardown_image(void)
{
}

int main()
{
  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kARGB_8888_Config, 800, 100);
  bitmap.allocPixels();
  SkBitmapDevice device(bitmap);
  SkCanvas canvas(&device);
  SkPaint paint;

  for (int i=0; i<1; i++) {
    if (sk_setup_image(&paint) != 1) {
      errx(1, "Failed to setup image\n");
    }
    /* time... */
    sk_test_image(&canvas, &paint);
    /* done timing */
  }

  sk_teardown_image();
  {
    SkAutoLockPixels image_lock(bitmap);
    cairo_surface_t* surface = cairo_image_surface_create_for_data((unsigned char*)bitmap.getPixels(), CAIRO_FORMAT_ARGB32,
								   bitmap.width(), bitmap.height(), bitmap.rowBytes());
    cairo_surface_write_to_png(surface, "image.png");
    cairo_surface_destroy(surface);
  }
 
  return 0;
}
