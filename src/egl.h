#ifndef __EGL_H__  // -*- mode: c++ -*-
#define __EGL_H__

#include <X11/Xlib.h>
#include <EGL/egl.h>

struct _device_config;
typedef struct _device_config device_config_t;

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
