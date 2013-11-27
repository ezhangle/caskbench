#include <SkCanvas.h>
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
  ctx->skia_paint->setMaskFilter(mask);
  return 1;
}

void
sk_teardown_mask(void)
{
}

int
sk_test_mask(caskbench_context_t *ctx)
{
  // TODO: Apply mask
  return 1;
}
