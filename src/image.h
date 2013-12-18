#ifndef __EGL_H__    // -*- mode: c++ -*-
#define __EGL_H__

#include <X11/Xlib.h>

typedef struct {
    Display   *dpy;
    Window     window;
} image_state_t;

void cleanup_state_image(void *data);

#endif // __EGL_H__
