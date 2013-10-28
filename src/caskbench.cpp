#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <cairo.h>

#include "caskbench.h"

typedef struct _caskbench_perf_case {
  const char *name;
  int (*setup)(cairo_t*);
  void (*teardown)(void);
  int (*test)(cairo_t*);
} caskbench_perf_case_t;

typedef struct _caskbench_result {
  const char *test_name;
  int iterations;
  double min_run_time;
  double avg_run_time;
  int status;
} caskbench_result_t;

caskbench_perf_case_t perf_cases[] = {
  {"fill", setup_fill,   teardown_fill,   test_fill},
  {"image", setup_image,  teardown_image,  test_image},
  {"mask", setup_mask,   teardown_mask,   test_mask},
  {"paint", setup_paint,  teardown_paint,  test_paint},
  {"stroke", setup_stroke, teardown_stroke, test_stroke},
};
#define NUM_CASES (5)

double
get_tick (void)
{
  struct timeval now;
  gettimeofday (&now, NULL);
  return (double)now.tv_sec + (double)now.tv_usec / 1000000.0;
}

void
randomize_color(cairo_t *cr)
{
  double red, green, blue, alpha;

  red = (double)rand()/RAND_MAX;
  green = (double)rand()/RAND_MAX;
  blue = (double)rand()/RAND_MAX;
  alpha = (double)rand()/RAND_MAX;

  cairo_set_source_rgba (cr, red, green, blue, alpha);
}

void
display_results_json(const caskbench_result_t *result)
{
  printf("   {\n");
  printf("       \"test\": \"%s\",\n", result->test_name);
  printf("       \"iterations\": %d,\n", result->iterations);
  printf("       \"minimum run time (s)\": %f,\n", result->min_run_time);
  printf("       \"average run time (s)\": %f,\n", result->avg_run_time);
  printf("       \"status\": \"%d\"\n", result->status);
  printf("   }");
}

// TODO: parse_options

int
main (int argc, char *argv[])
{
  int c, i;
  int iterations_requested = 16*1024;
  double start_time, stop_time, run_time, run_total;

  // TODO: Open output file
  // start json
  printf("[\n");
  for (c=0; c<NUM_CASES; c++) {
    // Setup
    caskbench_result_t result;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 800, 80);
    cairo_t *cr = cairo_create(surface);
    // If dry run, just list the test
    srand(0xdeadbeef);

    result.test_name = perf_cases[c].name;
    result.min_run_time = -1.0;
    result.avg_run_time = -1.0;

    if (perf_cases[c].setup != NULL)
      if (!perf_cases[c].setup(cr)) {
	result.status = -1;
	goto FINAL;
      }

    // TODO: Perhaps run once just to warm caches and calibrate
    for (i=iterations_requested; i>=0; --i) {
      try {
	assert(perf_cases[c].test);
	start_time = get_tick();

	// Execute test_case
	result.status = perf_cases[c].test(cr);

	stop_time = get_tick();
	run_time = stop_time - start_time;
	if (result.min_run_time < 0)
	  result.min_run_time = run_time;
	else
	  result.min_run_time = MIN(run_time, result.min_run_time);
	run_total += run_time;
	printf("%f\n", run_time);
      } catch (...) {
	printf("Unknown exception encountered\n");
	// Mark as crashed
	result.status = 3; // ERROR
	goto FINAL;
      }
    }
    result.iterations = iterations_requested - i;
    result.avg_run_time = run_total / (double)result.iterations;

    cairo_surface_write_to_png (surface, "fill.png");

  FINAL:
    // TODO: Print results to stdout plain text
    display_results_json(&result);
    if (c != NUM_CASES-1)
      printf(",");
    printf("\n");

    if (perf_cases[c].teardown != NULL)
      perf_cases[c].teardown();
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
  }

  // End json
  printf("]\n");
  // TODO: close output file
}
