#include <err.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

#include "glx.h"

static glx_state_t *state;

cairo_surface_t *
create_cairo_surface_glx (int width, int height)
{
    cairo_device_t *cairo_device;
    cairo_surface_t *cairo_surface;

    state = (glx_state_t*) malloc (sizeof (glx_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!createGLXContextAndWindow(state, width, height)) {
        warnx ("Could not create GLX context and window\n");
        cleanup_state_glx(state);
        return NULL;
    }

    cairo_device = cairo_glx_device_create (state->dpy, state->glx_context);
    cairo_gl_device_set_thread_aware (cairo_device, 0);
    cairo_surface = cairo_gl_surface_create_for_window (cairo_device,
                                                        state->window,
                                                        width,
                                                        height);
    cairo_device_destroy (cairo_device);

    return cairo_surface;
}

void
destroy_cairo_glx(void)
{
    cleanup_state_glx(state);
}

void
update_cairo_glx(void)
{
}
