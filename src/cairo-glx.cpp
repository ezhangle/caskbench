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

#include "glx.h"
#include "device_config.h"

static glx_state_t *state;

cairo_surface_t *
create_cairo_surface_glx (const device_config_t& config)
{
    cairo_device_t *cairo_device;
    cairo_surface_t *cairo_surface;

    state = (glx_state_t*) malloc (sizeof (glx_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!createGLXContextAndWindow(state, config.width, config.height)) {
        warnx ("Could not create GLX context and window\n");
        cleanup_state_glx(state);
        return NULL;
    }

    cairo_device = cairo_glx_device_create (state->dpy, state->glx_context);
    cairo_gl_device_set_thread_aware (cairo_device, 0);
    cairo_surface = cairo_gl_surface_create_for_window (cairo_device,
                                                        state->window,
                                                        config.width,
                                                        config.height);
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
