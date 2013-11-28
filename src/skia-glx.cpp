#include <err.h>
#include <stdlib.h>

#include <GrGLInterface.h>
#include <GrRenderTarget.h>
#include <GrContext.h>
#include <SkGpuDevice.h>

#include "glx.h"

static void
cleanup (void *data)
{
  glx_state_t *state = (glx_state_t*)data;
  destroyGLXContextAndWindow(state);
  free(state);
}

SkBaseDevice *
create_skia_device_glx (int width, int height)
{
  glx_state_t *state;

  GrBackendRenderTargetDesc desc;
  GrContext* ctx;
  GrRenderTarget* target;
  SkGpuDevice *device;

  state = (glx_state_t*) malloc (sizeof (glx_state_t));
  if (!state) {
    warnx ("Out of memory\n");
    return NULL;
  }

  if (!createGLXContextAndWindow(state, width, height)) {
    warnx ("Could not create GLX context and window\n");
    return NULL;
  }

  cairo_device = cairo_glx_device_create (state->dpy, state->glx_context);
  cairo_gl_device_set_thread_aware (cairo_device, 0);
  cairo_surface = cairo_gl_surface_create_for_window (cairo_device,
						      state->window,
						      width,
						      height);

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
  /*
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
