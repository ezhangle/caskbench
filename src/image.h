/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef __IMAGE_H__    // -*- mode: c++ -*-
#define __IMAGE_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <device_config.h>

typedef struct {
    Display   *dpy;
    Window     window;
    GC gc;
    XImage image;
    int width;
    int height;
} image_state_t;

void cleanup_state_image(void *data);
bool createImageWindow(image_state_t *state, const device_config_t& device_config);
void destroyImageWindow(image_state_t *state);
void updateImageWindow(image_state_t *state);

#endif // __IMAGE_H__
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
