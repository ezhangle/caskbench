/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef __DEVICE_CONFIG_H__    // -*- mode: c++ -*-
#define __DEVICE_CONFIG_H__

typedef struct _device_config {

    // General Settings
    const char *surface_type;
    int width;
    int height;

    // EGL Settings
    int egl_samples;
    int egl_sample_buffers;
    int egl_msaa_sample_count;

} device_config_t;

#endif // __DEVICE_CONFIG_H__
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
