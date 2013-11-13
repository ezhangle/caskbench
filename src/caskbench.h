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

#ifndef NUM_ELEM
# define NUM_ELEM(x) (sizeof(x)/sizeof(x[0]))
#endif

class SkPaint;
class SkCanvas;

typedef struct _caskbench_context {
  int       size;
  int       canvas_width;
  int       canvas_height;
  cairo_t   *cr;
  SkPaint   *paint;
  SkCanvas  *canvas;
} caskbench_context_t;

// Framework
double get_tick (void);
void   display_results_json(int iterations, int status);

// Cairo helpers
void   randomize_color(cairo_t *cr);

// Cairo Tests
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

int  ca_setup_stroke(caskbench_context_t *ctx);
void ca_teardown_stroke();
int  ca_test_stroke(caskbench_context_t *ctx);

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

int  ca_setup_rectangles(caskbench_context_t *ctx);
void ca_teardown_rectangles();
int  ca_test_rectangles(caskbench_context_t *ctx);

int  ca_setup_roundrect(caskbench_context_t *ctx);
void ca_teardown_roundrect();
int  ca_test_roundrect(caskbench_context_t *ctx);

int  ca_setup_bubbles(caskbench_context_t *ctx);
void ca_teardown_bubbles();
int  ca_test_bubbles(caskbench_context_t *ctx);

int  ca_setup_multishape(caskbench_context_t *ctx);
void ca_teardown_multishape();
int  ca_test_multishape(caskbench_context_t *ctx);


// Skia Tests
int  sk_setup_fill(caskbench_context_t *ctx);
void sk_teardown_fill();
int  sk_test_fill(caskbench_context_t *ctx);

int  sk_setup_image(caskbench_context_t *ctx);
void sk_teardown_image();
int  sk_test_image(caskbench_context_t *ctx);

int  sk_setup_mask(caskbench_context_t *ctx);
void sk_teardown_mask();
int  sk_test_mask(caskbench_context_t *ctx);

int  sk_setup_paint(caskbench_context_t *ctx);
void sk_teardown_paint();
int  sk_test_paint(caskbench_context_t *ctx);

int  sk_setup_stroke(caskbench_context_t *ctx);
void sk_teardown_stroke();
int  sk_test_stroke(caskbench_context_t *ctx);

int  sk_setup_rectangles(caskbench_context_t *ctx);
void sk_teardown_rectangles();
int  sk_test_rectangles(caskbench_context_t *ctx);

int  sk_setup_roundrect(caskbench_context_t *ctx);
void sk_teardown_roundrect();
int  sk_test_roundrect(caskbench_context_t *ctx);

int  sk_setup_bubbles(caskbench_context_t *ctx);
void sk_teardown_bubbles();
int  sk_test_bubbles(caskbench_context_t *ctx);

int  sk_setup_multishape(caskbench_context_t *ctx);
void sk_teardown_multishape();
int  sk_test_multishape(caskbench_context_t *ctx);

#endif // __CASKBENCH_H__
