#ifndef __CASKBENCH_H__    // -*- mode: c++ -*-
#define __CASKBENCH_H__

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef NUM_ELEM
# define NUM_ELEM(x) (sizeof(x)/sizeof(x[0]))
#endif

class SkPaint;
class SkCanvas;
class SkBaseDevice;

typedef struct _caskbench_context {
    int              size;
    int              canvas_width;
    int              canvas_height;

    cairo_t         *cairo_cr;
    cairo_surface_t *cairo_surface;

    SkPaint         *skia_paint;
    SkCanvas        *skia_canvas;
    SkBaseDevice    *skia_device;

    cairo_surface_t *(*setup_cairo)(int w, int h);
    SkBaseDevice    *(*setup_skia)(int w, int h);
    void             (*destroy_cairo)(void);
    void             (*destroy_skia)(void);

} caskbench_context_t;

// Framework
double get_tick (void);
void   display_results_json(int iterations, int status);

// Cairo helpers
void   randomize_color(cairo_t *cr);

#endif // __CASKBENCH_H__
