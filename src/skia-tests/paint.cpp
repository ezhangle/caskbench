// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include "SkRect.h"
#include <cairo.h>
 
#include "caskbench.h"

static SkRect r;

int
sk_setup_fill(SkPaint *paint)
{
  return 1;
}

int
sk_test_fill(SkCanvas *canvas, SkPaint *paint)
{
  int i;
  for (i=0; i<32; i++) {
    canvas->drawColor(rand());
  }

  return 1;
}

void
sk_teardown_fill(void)
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
    sk_setup_fill(&paint);
    /* time... */
    sk_test_fill(&canvas, &paint);
    /* done timing */
  }

  sk_teardown_fill();
  {
    SkAutoLockPixels image_lock(bitmap);
    cairo_surface_t* surface = cairo_image_surface_create_for_data((unsigned char*)bitmap.getPixels(), CAIRO_FORMAT_ARGB32,
								   bitmap.width(), bitmap.height(), bitmap.rowBytes());
    cairo_surface_write_to_png(surface, "fill.png");
    cairo_surface_destroy(surface);
  }
 
  return 0;
}
