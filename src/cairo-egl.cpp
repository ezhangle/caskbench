/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <err.h>
#include <stdlib.h>

#include <cairo.h>
#include <cairo-gl.h>

#if HAVE_GLES3_H
#include <GLES3/gl3.h>
#elif HAVE_GLES2_H
#include <GLES2/gl2.h>
#endif

#include "egl.h"
#include "device_config.h"

static egl_state_t *state;
static cairo_surface_t *cairo_surface;

cairo_surface_t *
create_cairo_surface_egl (const device_config_t& config)
{
    cairo_device_t *cairo_device;

    state = (egl_state_t*) malloc (sizeof (egl_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!createEGLContextAndWindow(state, config)) {
        cleanup_state_egl(state);
        errx (-1, "Could not create EGL context and window\n");
    }

    eglMakeCurrent(state->egl_display, state->egl_surface, state->egl_surface, state->egl_context);

    cairo_device = cairo_egl_device_create (state->egl_display, state->egl_context);
    cairo_status_t status = cairo_device_status (cairo_device);
    if (status != CAIRO_STATUS_SUCCESS)
        errx (-1, "Could not create EGL device: (%d) %s\n", status, cairo_status_to_string (status) );

    cairo_gl_device_set_thread_aware (cairo_device, 0);
    cairo_surface = cairo_gl_surface_create_for_egl (cairo_device,
                                                     state->egl_surface,
                                                     config.width, config.height);

    cairo_device_destroy (cairo_device);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    return cairo_surface;
}

void
destroy_cairo_egl(void)
{
    //cairo_surface_destroy (cairo_surface);
    cleanup_state_egl (state);
}

void
update_cairo_egl(void)
{
    //eglSwapBuffers(state->egl_display, state->egl_surface);
    cairo_gl_surface_swapbuffers (cairo_surface);
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
