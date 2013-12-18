#include <err.h>
#include <stdlib.h>

#include <GrGLInterface.h>
#include <GrRenderTarget.h>
#include <GrContext.h>
#include <SkGpuDevice.h>

#include "glx.h"

static glx_state_t *state;

SkBaseDevice *
create_skia_device_glx (int width, int height)
{
    GrBackendRenderTargetDesc desc;
    GrContext* ctx;
    GrRenderTarget* target;
    SkGpuDevice *device;

    state = (glx_state_t*) malloc (sizeof (glx_state_t));
    if (!state) {
        warnx("Out of memory\n");
        return NULL;
    }

    if (!createGLXContextAndWindow(state, width, height)) {
        warnx("Could not create GLX context and window\n");
        cleanup_state_glx(state);
        return NULL;
    }

    desc.fWidth = width;
    desc.fHeight = height;
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    //desc.fOrigin = kTopLeft_GrSurfaceOrigin;
    desc.fSampleCnt = 4;
    desc.fStencilBits = 1;
    desc.fRenderTargetHandle = 0;
    ctx = GrContext::Create(kOpenGL_GrBackend, 0);
    target = ctx->wrapBackendRenderTarget(desc);

    device = new SkGpuDevice(ctx, target);
    return device;
}

void
destroy_skia_glx(void)
{
    cleanup_state_glx(state);
}
