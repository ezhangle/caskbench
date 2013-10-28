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

// Framework
double get_tick (void);
void   display_results_json(int iterations, int status);

// Cairo helpers
void   randomize_color(cairo_t *cr);

// Tests
int setup_fill(cairo_t *cr);
void teardown_fill();
int test_fill(cairo_t *cr);

int setup_image(cairo_t *cr);
void teardown_image();
int test_image(cairo_t *cr);

int setup_mask(cairo_t *cr);
void teardown_mask();
int test_mask(cairo_t *cr);

int setup_paint(cairo_t *cr);
void teardown_paint();
int test_paint(cairo_t *cr);

int setup_stroke(cairo_t *cr);
void teardown_stroke();
int test_stroke(cairo_t *cr);

#endif // __CASKBENCH_H__
