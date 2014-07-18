/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */

#include <config.h>

#include <assert.h>
#include <err.h>
#include <string.h>
#include <forward.h>

#ifdef USE_CAIRO
#include <cairo.h>
#ifdef HAVE_CAIRO_GL_H
#  include <cairo-gl.h>
#endif
#endif

#ifdef USE_SKIA
#  include <SkBitmap.h>
#  include <SkBitmapDevice.h>
#  include <SkPaint.h>
#  include <SkCanvas.h>
#  include <SkDeferredCanvas.h>
#  include <SkImage.h>
#  include <SkSurface.h>
#  include <GrRenderTarget.h>
#endif

#include "caskbench_context.h"
#include "device_config.h"

void
context_init(caskbench_context_t *context, int size)
{
    assert(context);
    assert(size > 0);
    memset(context, 0, sizeof(caskbench_context_t));
    context->size = size;
    context->canvas_width = 800;
    context->canvas_height = 400;
    context->tolerance = 0.25;
}

#ifdef USE_CAIRO
void
context_setup_cairo(caskbench_context_t *context, const device_config_t& config)
{
    if (config.surface_type == NULL || !strncmp(config.surface_type, "image", 5)) {
        context->setup_cairo = create_cairo_surface_image;
        context->destroy_cairo = destroy_cairo_image;
        context->update_cairo = update_cairo_image;
    } else if (!strncmp(config.surface_type, "glx", 3)) {
#if USE_GLX
        context->setup_cairo = create_cairo_surface_glx;
        context->destroy_cairo = destroy_cairo_glx;
        context->update_cairo = update_cairo_glx;
#else
        errx(1, "glx unsupported in this build\n");
#endif

    } else if (!strncmp(config.surface_type, "egl", 3)) {
#if USE_EGL
        context->setup_cairo = create_cairo_surface_egl;
        context->destroy_cairo = destroy_cairo_egl;
        context->update_cairo = update_cairo_egl;
#else
        errx(1, "egl unsupported in this build\n");
#endif
    }
    assert (context->setup_cairo);
    assert (context->destroy_cairo);
    assert (context->update_cairo);

    context->cairo_surface = context->setup_cairo(config);
    if (!context->cairo_surface)
        errx(2, "Could not create a cairo surface\n");

    context->cairo_cr = cairo_create(context->cairo_surface);

    // Clear background to black
    cairo_set_source_rgb (context->cairo_cr, 0, 0, 0);
    cairo_paint (context->cairo_cr);

    // Enable anti-aliasing
    cairo_set_antialias (context->cairo_cr, CAIRO_ANTIALIAS_DEFAULT);

    // Ease up Cairo's tessellation tolerance (default is 0.001)
    cairo_set_tolerance (context->cairo_cr, context->tolerance);
}
#endif

void
context_update_cairo(caskbench_context_t *context)
{
    context->update_cairo();
}

void
context_destroy_cairo(caskbench_context_t *context)
{
    if (!context)
        return;
#ifdef USE_CAIRO
    cairo_destroy(context->cairo_cr);
    cairo_surface_destroy(context->cairo_surface);
#endif
    context->destroy_cairo();
    context->cairo_cr = NULL;
}

#ifdef USE_SKIA
void
context_setup_skia(caskbench_context_t *context, const device_config_t& config)
{
    if (config.surface_type == NULL || !strncmp(config.surface_type, "image", 5)) {
        context->setup_skia = create_skia_device_image;
        context->destroy_skia = destroy_skia_image;
        context->update_skia = update_skia_image;
    } else if (!strncmp(config.surface_type, "glx", 3)) {
#if USE_GLX
        context->setup_skia = create_skia_device_glx;
        context->destroy_skia = destroy_skia_glx;
        context->update_skia = update_skia_glx;
#else
        errx(1, "glx unsupported in this build\n");
#endif

    } else if (!strncmp(config.surface_type, "egl", 3)) {
#if USE_EGL
        context->setup_skia = create_skia_device_egl;
        context->destroy_skia = destroy_skia_egl;
        context->update_skia = update_skia_egl;
#else
        errx(1, "egl unsupported in this build\n");
#endif
    }

    assert (context->setup_skia);
    assert (context->destroy_skia);
    assert (context->update_skia);

    context->skia_device = context->setup_skia(config);
    if (!context->skia_device)
        errx(2, "Could not create a skia device\n");

    context->skia_paint = new SkPaint;
    context->skia_canvas = new SkCanvas(context->skia_device);


    // egl and deferred_rendering
    if (context->is_egl_deferred) {
        SkDeferredCanvas *dCanvas;
        SkImageInfo dImageInfo;

        // create deferred canvas
        dImageInfo.fWidth = context->canvas_width;
        dImageInfo.fHeight = context->canvas_height;
        dImageInfo.fColorType = kRGBA_8888_SkColorType;
        dImageInfo.fAlphaType = kPremul_SkAlphaType;
        context->dSurface = SkSurface::NewRenderTarget(context->skia_device->accessRenderTarget()->getContext(), dImageInfo, 0);
        dCanvas = SkDeferredCanvas::Create(context->dSurface);
        context->skia_canvas_immediate = context->skia_canvas;
        context->skia_canvas = dCanvas;
    }
    // Clear background to black
    context->skia_canvas->clear(0);
    context->skia_paint->setARGB(255, 0, 0, 0);
    context->skia_canvas->drawPaint(*context->skia_paint);

    // Enable anti-aliasing
    context->skia_paint->setAntiAlias(true);
}

void
context_update_skia(caskbench_context_t *context)
{
    // egl and deferred_rendering
    if (context->is_egl_deferred) {
        SkPaint p;
        if (context->snapshot)
            context->snapshot->unref();

        context->snapshot = static_cast <SkDeferredCanvas*> (context->skia_canvas)->newImageSnapshot();
        SkRect src_rect, dst_rect;
        src_rect = SkRect::MakeXYWH(0, 0, context->canvas_width, context->canvas_height);
        dst_rect = src_rect;

        p.setXfermodeMode(SkXfermode::kSrc_Mode);
        context->snapshot->draw(context->skia_canvas_immediate, &src_rect, dst_rect, &p);

    }
    context->update_skia();
}

void
context_destroy_skia(caskbench_context_t *context)
{
    if (!context)
        return;
    delete context->skia_paint;
    delete context->skia_canvas_immediate;
    delete context->skia_canvas;
    delete context->skia_device;
    delete context->dSurface;
    delete context->snapshot;
    context->destroy_skia();
    context->skia_device = NULL;
}
#endif
