#ifndef __CASKBENCH_H__
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

class SkPaint;
class SkCanvas;

typedef struct _caskbench_context {
  cairo_t   *cr;
  SkPaint   *paint;
  SkCanvas  *canvas;
} caskbench_context_t;

// Framework
double get_tick (void);
void   display_results_json(int iterations, int status);

// Cairo helpers
void   randomize_color(cairo_t *cr);

// Tests
int  ca_setup_fill(caskbench_context_t *ctx);
void ca_teardown_fill();
int  ca_test_fill(caskbench_context_t *ctx);

int  ca_setup_image(caskbench_context_t *ctx);
void ca_teardown_image();
int  ca_test_image(caskbench_context_t *ctx);

int  ca_setup_mask(caskbench_context_t *ctx);
void ca_teardown_mask();
int  ca_test_mask(caskbench_context_t *ctx);

int  ca_setup_paint(caskbench_context_t *ctx);
void ca_teardown_paint();
int  ca_test_paint(caskbench_context_t *ctx);

int  ca_setup_stroke(caskbench_context_t *ctx);
void ca_teardown_stroke();
int  ca_test_stroke(caskbench_context_t *ctx);

#endif // __CASKBENCH_H__
