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
