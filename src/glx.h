#ifndef __EGL_H__    // -*- mode: c++ -*-
#define __EGL_H__

#include <X11/Xlib.h>
#include <GL/glx.h>


typedef struct {
    Display   *dpy;
    Window     window;

    GLXContext glx_context;
} glx_state_t;

bool createGLXContextAndWindow(glx_state_t *state, int width, int height);
void destroyGLXContextAndWindow(glx_state_t *state);
void cleanup_state_glx(void *state);

#endif // __EGL_H__
