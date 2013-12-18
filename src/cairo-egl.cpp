#include <err.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-gl.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "egl.h"

static egl_state_t *state;

cairo_surface_t *
create_cairo_surface_egl (int width, int height)
{
    cairo_device_t *cairo_device;
    cairo_surface_t *cairo_surface;

    state = (egl_state_t*) malloc (sizeof (egl_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!createEGLContextAndWindow(state, width, height)) {
        cleanup_state_egl(state);
        return NULL;
    }

    cairo_device = cairo_egl_device_create (state->egl_display, state->egl_context);
    cairo_gl_device_set_thread_aware (cairo_device, 0);
    cairo_surface = cairo_gl_surface_create_for_egl (cairo_device,
                                                     state->egl_surface,
                                                     width, height);

    cairo_device_destroy (cairo_device);

    return cairo_surface;
}

void
destroy_cairo_egl(void)
{
    cleanup_state_egl(state);
}
