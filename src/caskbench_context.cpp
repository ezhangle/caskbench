#include <config.h>
#include <assert.h>
#include <err.h>
#include "caskbench_context.h"

#include <cairo.h>
#ifdef HAVE_CAIRO_GL_H
#  include <cairo-gl.h>
#endif

#ifdef USE_SKIA
#  include <SkBitmap.h>
#  include <SkBitmapDevice.h>
#  include <SkPaint.h>
#  include <SkCanvas.h>
#endif

void
context_init(caskbench_context_t *context, int size)
{
    assert(context);
    assert(size > 0); 

    context->size = size;
    context->canvas_width = 800;
    context->canvas_height = 400;
    context->skia_device = NULL;
    context->skia_paint = NULL;
    context->setup_cairo = NULL;
    context->setup_skia = NULL;
    context->destroy_cairo = NULL;
    context->destroy_skia = NULL;
    context->update_cairo = NULL;
    context->update_skia = NULL;
}

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
    cairo_set_tolerance (context->cairo_cr, 0.25);
}

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
    cairo_destroy(context->cairo_cr);
    cairo_surface_destroy(context->cairo_surface);
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
    context->update_skia();
}

void
context_destroy_skia(caskbench_context_t *context)
{
    if (!context)
        return;
    delete context->skia_paint;
    delete context->skia_canvas;
    delete context->skia_device;
    context->destroy_skia();
    context->skia_device = NULL;
}
#endif

