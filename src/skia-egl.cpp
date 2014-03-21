#include <config.h>

#include <err.h>
#include <stdlib.h>

#include <GrGLInterface.h>
#include <GrRenderTarget.h>
#include <GrContext.h>
#include <SkGpuDevice.h>

#include <EGL/egl.h>
#if HAVE_GLES3_H
#include <GLES3/gl3.h>
#elif HAVE_GLES2_H
#include <GLES2/gl2.h>
#endif


#include "egl.h"

static egl_state_t *state;

SkBaseDevice *
create_skia_device_egl (int width, int height)
{
    GrBackendRenderTargetDesc desc;
    GrContext* ctx;
    GrRenderTarget* target;
    SkGpuDevice *device;

    state = (egl_state_t*) malloc (sizeof (egl_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!createEGLContextAndWindow(state, width, height)) {
        cleanup_state_egl(state);
        return NULL;
    }

    eglMakeCurrent(state->egl_display, state->egl_surface, state->egl_surface, state->egl_context);

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
destroy_skia_egl(void)
{
    cleanup_state_egl(state);
}

void
update_skia_egl(void)
{
}

