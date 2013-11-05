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
int  ca_setup_fill(cairo_t *cr);
void ca_teardown_fill();
int  ca_test_fill(cairo_t *cr);

int  ca_setup_image(cairo_t *cr);
void ca_teardown_image();
int  ca_test_image(cairo_t *cr);

int  ca_setup_mask(cairo_t *cr);
void ca_teardown_mask();
int  ca_test_mask(cairo_t *cr);

int  ca_setup_paint(cairo_t *cr);
void ca_teardown_paint();
int  ca_test_paint(cairo_t *cr);

int  ca_setup_stroke(cairo_t *cr);
void ca_teardown_stroke();
int  ca_test_stroke(cairo_t *cr);

#endif // __CASKBENCH_H__
