#include <err.h>
#include <stdlib.h>

#include <GrGLInterface.h>
#include <GrRenderTarget.h>
#include <GrContext.h>
#include <SkGpuDevice.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "egl.h"

static void
cleanup (void *data)
{
  egl_state_t *state = (egl_state_t*)data;
  destroyEGLContextAndWindow (state);
  free (state);
}

SkBaseDevice *
create_skia_device_egl (int width, int height)
{
  egl_state_t *state;

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
    cleanup(state);
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

  /* TODO: Set cleanup routine to get called

  if (cairo_device_set_user_data (cairo_device,
				  (cairo_user_data_key_t *) cleanup,
				  state,
				  cleanup))
    {
      warnx ("Failed to setup cleanup callback closure\n");
      cleanup (state);
      return NULL;
    }

  cairo_device_destroy (cairo_device);
  */

  return device;
}
