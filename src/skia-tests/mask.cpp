#include <config.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <effects/SkStippleMaskFilter.h>

#include "caskbench.h"
#include "skia-shapes.h"

int
sk_setup_mask(caskbench_context_t *ctx)
{
#if USE_LEGACY_SKIA_SRA
    SkMaskFilter *mask = new SkStippleMaskFilter();
#else
    SkMaskFilter *mask = SkStippleMaskFilter::Create();
#endif
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
    int i;

    for (i=0; i<ctx->size; i++) {
        skiaRandomizeColor(ctx);

        // Apply mask on a circle
        ctx->skia_canvas->drawCircle(40*i, 40, 30, *ctx->skia_paint);
    }

    return 1;
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
