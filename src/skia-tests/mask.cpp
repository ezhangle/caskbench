// From http://www.atoker.com/blog/2008/09/06/skia-graphics-library-in-chrome-first-impressions/

#include <err.h>

#include <SkBitmap.h>
#include <SkBitmapDevice.h>
#include <SkPaint.h>
#include <effects/SkBlurMaskFilter.h>

#include <cairo.h>
 
#include "caskbench.h"

int
sk_setup_mask(caskbench_context_t *ctx)
{
  SkMaskFilter *mask = SkBlurMaskFilter::Create(1.0,
						SkBlurMaskFilter::kNormal_BlurStyle,
						SkBlurMaskFilter::kHighQuality_BlurFlag);
  ctx->paint->setMaskFilter(mask);
  return 1;
}

int
sk_test_mask(caskbench_context_t *ctx)
{
  // TODO: Apply mask
  return 1;
}

void
sk_teardown_mask(void)
{
}

