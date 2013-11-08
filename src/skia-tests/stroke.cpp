// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include "SkBitmap.h"
#include "SkBitmapDevice.h"
#include "SkPaint.h"
#include "SkRect.h"
#include <cairo.h>
 
#include "caskbench.h"

static SkRect r;

int
setup_stroke(SkPaint *paint)
{
  paint->setARGB(255, 255, 255, 255);
  r.set(10, 10, 20, 20);
  return 1;
}

int
test_stroke(SkCanvas *canvas, SkPaint *paint)
{
  int i, x, w, prev_w;
  x = 0;
  prev_w = 0;
  for (i=0; i<32; i++) {
    paint->setColor(rand());
    w = (32.0*rand())/RAND_MAX + 1;
    x += 4 + (prev_w + w)/2.0;
    paint->setStrokeWidth(w);
    canvas->drawLine(x,10, x,70, *paint);
    prev_w = w;
  }

  return 1;
}

void
teardown_stroke(void)
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
    setup_stroke(&paint);
    test_stroke(&canvas, &paint);
  }

  {
    SkAutoLockPixels image_lock(bitmap);
    cairo_surface_t* surface = cairo_image_surface_create_for_data((unsigned char*)bitmap.getPixels(), CAIRO_FORMAT_ARGB32,
								   bitmap.width(), bitmap.height(), bitmap.rowBytes());
    cairo_surface_write_to_png(surface, "snapshot.png");
    cairo_surface_destroy(surface);
  }
 
  return 0;
}
