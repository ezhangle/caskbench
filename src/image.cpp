/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <err.h>
#include <stdlib.h>

#include "device_config.h"
#include "image.h"

void
cleanup_state_image (void *data)
{
    image_state_t *state = (image_state_t*)data;
    free(state);
}

bool
createImageWindow(image_state_t *state, const device_config_t& device_config)
{
    state->dpy = XOpenDisplay (NULL);
    if (!state->dpy) {
        warnx ("fail to open display: %s\n", XDisplayName (0));
        return false;
    }

    int screen = DefaultScreen (state->dpy);

    XVisualInfo vi;
    XSetWindowAttributes attrs;

    XMatchVisualInfo (state->dpy, screen, 32, TrueColor, &vi);
    attrs.border_pixel = 0;
    attrs.event_mask = StructureNotifyMask;
    attrs.colormap = XCreateColormap (state->dpy, DefaultRootWindow (state->dpy),
                                      vi.visual, AllocNone);
    state->window = XCreateWindow (state->dpy, DefaultRootWindow (state->dpy),
                                   0, 0,
                                   device_config.width, device_config.height,
                                   0,
                                   vi.depth,
                                   InputOutput, vi.visual,
                                   CWBorderPixel | CWColormap | CWEventMask,
                                   &attrs);
    state->width = device_config.width;
    state->height = device_config.height;
    XMapWindow (state->dpy, state->window);

    state->gc = XCreateGC (state->dpy, state->window, 0, NULL);
    return true;
}

void
updateImageWindow(image_state_t* state)
{
    XPutImage (state->dpy, state->window, state->gc, &state->image,
               0, 0,
               0, 0,
               state->width, state->height);
    XSync (state->dpy, False);
}
void
destroyImageWindow(image_state_t* state)
{
    XFreeGC (state->dpy, state->gc);
    XDestroyWindow (state->dpy, state->window);
    XCloseDisplay (state->dpy);
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
