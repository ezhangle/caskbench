// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include "err.h"
#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include "SkImageDecoder.h"
#include "effects/SkBlurMaskFilter.h"
#include <cairo.h>
 
#include "caskbench.h"

int
sk_setup_mask(SkPaint *paint)
{
  SkMaskFilter *mask = SkBlurMaskFilter::Create(1.0,
						SkBlurMaskFilter::kNormal_BlurStyle,
						SkBlurMaskFilter::kHighQuality_BlurFlag);
  paint->setMaskFilter(mask);
  return 1;
}

int
sk_test_mask(SkCanvas *canvas, SkPaint *paint)
{
  // TODO: Apply mask
  return 1;
}

void
sk_teardown_mask(void)
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
    if (sk_setup_mask(&paint) != 1) {
      errx(1, "Failed to setup image\n");
    }
    /* time... */
    sk_test_mask(&canvas, &paint);
    /* done timing */
  }

  sk_teardown_mask();
  {
    SkAutoLockPixels image_lock(bitmap);
    cairo_surface_t* surface = cairo_image_surface_create_for_data((unsigned char*)bitmap.getPixels(), CAIRO_FORMAT_ARGB32,
								   bitmap.width(), bitmap.height(), bitmap.rowBytes());
    cairo_surface_write_to_png(surface, "mask.png");
    cairo_surface_destroy(surface);
  }
 
  return 0;
}
