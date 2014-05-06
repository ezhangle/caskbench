#ifndef __EGL_H__  // -*- mode: c++ -*-
#define __EGL_H__

#include <X11/Xlib.h>
#include <EGL/egl.h>

#include "forward.h"

typedef struct {
    Display   *dpy;
    Window     window;

    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;
} egl_state_t;

bool createEGLContextAndWindow(egl_state_t *state, const device_config_t& device_config);
void destroyEGLContextAndWindow(egl_state_t *state);
void cleanup_state_egl(void *data);

#endif // __EGL_H__
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
