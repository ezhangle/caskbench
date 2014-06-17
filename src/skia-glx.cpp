/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
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

    if (!createGLXContextAndWindow(state, config.width, config.height)) {
        cleanup_state_glx(state);
        errx("Could not create GLX context and window\n");
    }

    // TODO: See SkOSWindow_Unix.cpp

    glXMakeCurrent(state->dpy, state->window, state->glx_context);

    desc.fWidth = config.width;
    desc.fHeight = config.height;
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    desc.fStencilBits = 1;
    desc.fRenderTargetHandle = 0;
    desc.fSampleCnt = 0;

    ctx = GrContext::Create(kOpenGL_GrBackend, 0);
    if (!ctx) {
        cleanup_state_glx(state);
        errx("Could not create Gr context for kOpenGL_GrBackend\n");
    }

    target = ctx->wrapBackendRenderTarget(desc);
    device = new SkGpuDevice(ctx, target);

    //glClearStencil(0);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

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
    //glxSwapBuffers(state->glx_display, state->glx_surface); // TODO: ?
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
