#include <config.h>

#include <err.h>
#include <stdlib.h>

#include <GrGLInterface.h>
#include <GrRenderTarget.h>
#include <GrContext.h>
#include <SkGpuDevice.h>

#include "glx.h"
#include "device_config.h"

static glx_state_t *state;
GrContext* ctx;

static bool ctxErrorOccurred = false;
static int ctxErrorHandler(Display *dpy, XErrorEvent *ev)
{
    ctxErrorOccurred = true;
    return 0;
}

SkBaseDevice *
create_skia_device_glx (const device_config_t& config)
{
    GrBackendRenderTargetDesc desc;
    GrRenderTarget* target;
    SkGpuDevice *device;

    state = (glx_state_t*) malloc (sizeof (glx_state_t));
    if (!state) {
        warnx("Out of memory\n");
        return NULL;
    }

    // Install an X error handler so we won't exit if context allocation fails
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) =
        XSetErrorHandler(&ctxErrorHandler);

    warnx("Creating GLX context\n");
    if (!createGLXContextAndWindow(state, config.width, config.height)) {
        warnx("Could not create GLX context and window\n");
        cleanup_state_glx(state);
        return NULL;
    }

    // Sync to ensure errors get processed
    XSync(state->dpy, False);
    XSetErrorHandler(oldHandler);

    if (ctxErrorOccurred || !state->glx_context) {
        warnx("Failed to create an OpenGL context\n");
        cleanup_state_glx(state);
        return NULL;
    }

    glXMakeCurrent(state->dpy, state->window, state->glx_context);
    glClearColor(0, 0, 0, 0);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    ctx = GrContext::Create(kOpenGL_GrBackend, 0);
    if (ctx == NULL) {
        warnx("Could not create Gr context for kOpenGL_GrBackend\n");
        return NULL;
    }

    // TODO: See SkOSWindow_Unix.cpp

    desc.fWidth = config.width;
    desc.fHeight = config.height;
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    //desc.fOrigin = kTopLeft_GrSurfaceOrigin;
    desc.fSampleCnt = 4;
    desc.fStencilBits = 1;
    desc.fRenderTargetHandle = 0;
    target = ctx->wrapBackendRenderTarget(desc);

    device = new SkGpuDevice(ctx, target);
    return device;
}

void
destroy_skia_glx(void)
{
    delete ctx;
    cleanup_state_glx(state);
}

void
update_skia_glx(void)
{
    ctx->flush();
}

