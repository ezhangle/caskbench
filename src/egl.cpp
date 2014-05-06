/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <err.h>
#include <stdlib.h>

#include <EGL/egl.h>
#if HAVE_GLES3_H
#include <GLES3/gl3.h>
#elif HAVE_GLES2_H
#include <GLES2/gl2.h>
#endif

#include <unistd.h>

#include "egl.h"
#include "device_config.h"

bool
createEGLContextAndWindow(egl_state_t *state, const device_config_t& device_config)
{
    EGLint attr[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_ALPHA_SIZE, 1,
        EGL_STENCIL_SIZE, 1,
        EGL_SAMPLES, 0,
        EGL_SAMPLE_BUFFERS, 0,
        EGL_NONE
    };
    EGLint ctx_attr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLConfig egl_config;
    EGLint num;
    EGLint major, minor;

    // TODO: Hack
    // User configuration settings
    for (int i=0; i<18; i+=2) {
        switch (attr[i]) {
        case EGL_SAMPLES:
            attr[i+1] = device_config.egl_samples;
            break;
        case EGL_SAMPLE_BUFFERS:
            attr[i+1] = device_config.egl_sample_buffers;
            break;
        }
    }

    state->dpy = XOpenDisplay (NULL);
    if (state->dpy == NULL) {
        warnx ("Failed to open display: %s\n", XDisplayName (0));
        return false;
    }

    state->window = XCreateSimpleWindow (state->dpy, DefaultRootWindow (state->dpy),
                                         200, 200,
                                         device_config.width, device_config.height,
                                         0,
                                         BlackPixel (state->dpy, DefaultRootWindow (state->dpy)),
                                         BlackPixel (state->dpy, DefaultRootWindow (state->dpy)));
    XMapWindow (state->dpy, state->window);

    state->egl_display = eglGetDisplay ((EGLNativeDisplayType) state->dpy);
    if (state->egl_display == EGL_NO_DISPLAY) {
        warnx ("Cannot get egl display\n");
        return false;
    }

    if (!eglInitialize(state->egl_display, &major, &minor)) {
        warnx ("Cannot initialize EGL\n");
        return false;
    }

    if (!eglBindAPI(EGL_OPENGL_ES_API)) {
        warnx ("Cannot bind EGL to GLES API\n");
        return false;
    }

    if (! eglChooseConfig (state->egl_display, attr, &egl_config, 1, &num)) {
        warnx ("Cannot choose EGL configuration\n");
        return false;
    }

    if (!egl_config or num < 1) {
        warnx ("No valid EGL configuration could be found\n");
        return false;
    }

    state->egl_context = eglCreateContext (state->egl_display, egl_config, NULL, ctx_attr);
    if (state->egl_context == EGL_NO_CONTEXT) {
        warnx ("Cannot create EGL context\n");
        return false;
    }

    state->egl_surface = eglCreateWindowSurface (state->egl_display, egl_config, (EGLNativeWindowType)state->window, NULL);
    if (state->egl_surface == EGL_NO_SURFACE) {
        warnx ("Cannot create EGL window surface\n");
        return false;
    }
    return true;
}

void
destroyEGLContextAndWindow (egl_state_t *state)
{
    if (state->egl_display) {
        eglMakeCurrent (state->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext (state->egl_display, state->egl_context);
        eglDestroySurface (state->egl_display, state->egl_surface);
        state->egl_context = EGL_NO_CONTEXT;
        state->egl_surface = EGL_NO_SURFACE;
        eglTerminate (state->egl_display);
    }

    if (state->dpy) {
        XUnmapWindow (state->dpy, state->window);
        XDestroyWindow (state->dpy, state->window);
        XCloseDisplay (state->dpy);
    }
}

void
cleanup_state_egl(void *data)
{
    egl_state_t *state = (egl_state_t*)data;
    destroyEGLContextAndWindow(state);
    free(state);
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
