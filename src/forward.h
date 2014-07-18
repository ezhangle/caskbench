/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef __FORWARD_H__    // -*- mode: c++ -*-
#define __FORWARD_H__

// Cairo forward declarations
struct _cairo;
typedef struct _cairo cairo_t;

struct _cairo_surface;
typedef struct _cairo_surface cairo_surface_t;


// Skia forward declarations
class SkPaint;
class SkCanvas;
class SkShader;
class SkBaseDevice;
class SkImage;
class SkSurface;


// Caskbench forward declarations
struct _caskbench_context;
typedef struct _caskbench_context caskbench_context_t;

struct _device_config;
typedef struct _device_config device_config_t;

#endif
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
