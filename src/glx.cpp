#include <err.h>
#include <stdlib.h>

#include "glx.h"

bool
createGLXContextAndWindow(glx_state_t *state, int width, int height)
{
    int rgba_attribs[] = {
        GLX_RGBA,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        GLX_ALPHA_SIZE, 1,
        GLX_DOUBLEBUFFER,
        None
    };
    XVisualInfo *visinfo;
    XSetWindowAttributes xattr;

    state->dpy = XOpenDisplay (NULL);
    if (state->dpy == NULL) {
        warnx ("Failed to open display: %s\n", XDisplayName (0));
        return false;
    }

    visinfo = glXChooseVisual (state->dpy, DefaultScreen (state->dpy), rgba_attribs);
    if (visinfo == NULL) {
        warnx ("Failed to create RGBA and double-buffered visuals\n");
        XCloseDisplay (state->dpy);
        return NULL;
    }

    xattr.colormap = XCreateColormap (state->dpy,
                                      RootWindow (state->dpy, visinfo->screen),
                                      visinfo->visual, AllocNone);
    xattr.border_pixel = 0;
    state->window = XCreateWindow (state->dpy,
                                   DefaultRootWindow (state->dpy),
                                   100, 50,
                                   width, height, 0, visinfo->depth,
                                   InputOutput, visinfo->visual,
                                   CWBorderPixel | CWColormap,
                                   &xattr);
    XMapWindow (state->dpy, state->window);

    state->glx_context = glXCreateContext (state->dpy, visinfo, NULL, True);
    XFree (visinfo);

    if (state->glx_context == NULL) {
        warnx ("Could not create glx context\n");
        XCloseDisplay (state->dpy);
        return false;
    }

    return true;
}

void
destroyGLXContextAndWindow (glx_state_t *state)
{
    if (state->dpy) {
        glXDestroyContext (state->dpy, state->glx_context);

        XDestroyWindow (state->dpy, state->window);
        XCloseDisplay (state->dpy);
    }
}

void
cleanup_state_glx (void *data)
{
    glx_state_t *state = (glx_state_t*)data;
    destroyGLXContextAndWindow(state);
    free(state);
}
