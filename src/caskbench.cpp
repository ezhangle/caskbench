#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <cairo.h>
#include <popt.h>

#include "caskbench.h"

typedef struct _caskbench_options {
  int dry_run;
  int iterations;
} caskbench_options_t;

typedef struct _caskbench_perf_test {
  const char *name;
  int (*setup)(cairo_t*);
  void (*teardown)(void);
  int (*test_case)(cairo_t*);
} caskbench_perf_test_t;

typedef struct _caskbench_result {
  const char *test_case_name;
  int iterations;
  double min_run_time;
  double avg_run_time;
  int status;
} caskbench_result_t;

caskbench_perf_test_t perf_tests[] = {
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
  printf("       \"test case\": \"%s\",\n", result->test_case_name);
  printf("       \"iterations\": %d,\n", result->iterations);
  printf("       \"minimum run time (s)\": %f,\n", result->min_run_time);
  printf("       \"average run time (s)\": %f,\n", result->avg_run_time);
  printf("       \"status\": \"%d\"\n", result->status);
  printf("   }");
}

int
main (int argc, char *argv[])
{
  int c, i;
  caskbench_options_t opt;
  double start_time, stop_time, run_time, run_total;

  poptContext pc;
  struct poptOption po[] = {
    {"iterations", 'i', POPT_ARG_INT, &opt.iterations, 0,
     "The number of times each test case should be run",
     NULL},
    {"dry-run", 'n', POPT_ARG_NONE, &opt.dry_run, 0,
     "Just list the selected test case names without executing",
     "num"},
    POPT_AUTOHELP
    {NULL}
  };
  pc = poptGetContext(NULL, argc, (const char **)argv, po, 0);
  poptSetOtherOptionHelp(pc, "[ARG...]");
  poptReadDefaultConfig(pc, 0);

  opt.dry_run = 0;
  opt.iterations = 1024;

  int rc = poptGetNextOpt(pc);
  while ((rc = poptGetNextOpt(pc)) >= 0) {
  }
  if (rc != -1) {
    // handle error
    switch(rc) {
    case POPT_ERROR_NOARG:
      printf("Argument missing for an option\n");
      exit(1);
    case POPT_ERROR_BADOPT:
      printf("Option's argument could not be parsed\n");
      exit(1);
    case POPT_ERROR_BADNUMBER:
    case POPT_ERROR_OVERFLOW:
      printf("Option could not be converted to number\n");
      exit(1);
    default:
      printf("Unknown error in option processing\n");
      exit(1);
    }
  }

  // Remaining args are the tests to be run, or all if this list is empty
  const char **tests = poptGetArgs(pc);

  // TODO: Open output file
  // start json
  printf("[\n");
  for (c=0; c<NUM_CASES; c++) {
    // Setup
    caskbench_result_t result;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 800, 80);
    cairo_t *cr = cairo_create(surface);

    // If dry run, just list the test cases
    if (opt.dry_run) {
      printf("%s\n", perf_tests[c].name);
      continue;
    }

    srand(0xdeadbeef);

    result.test_case_name = perf_tests[c].name;
    result.min_run_time = -1.0;
    result.avg_run_time = -1.0;

    if (perf_tests[c].setup != NULL)
      if (!perf_tests[c].setup(cr)) {
	result.status = -1;
	goto FINAL;
      }

    // TODO: Perhaps run once just to warm caches and calibrate
    for (i=opt.iterations; i>=0; --i) {
      try {
	assert(perf_tests[c].test_case);
	start_time = get_tick();

	// Execute test_case
	result.status = perf_tests[c].test_case(cr);

	stop_time = get_tick();
	run_time = stop_time - start_time;
	if (result.min_run_time < 0)
	  result.min_run_time = run_time;
	else
	  result.min_run_time = MIN(run_time, result.min_run_time);
	run_total += run_time;
      } catch (...) {
	printf("Unknown exception encountered\n");
	// Mark as crashed
	result.status = 3; // ERROR
	goto FINAL;
      }
    }
    result.iterations = opt.iterations - i;
    result.avg_run_time = run_total / (double)result.iterations;

    cairo_surface_write_to_png (surface, "fill.png");

  FINAL:
    // TODO: Print results to stdout plain text
    display_results_json(&result);
    if (c != NUM_CASES-1)
      printf(",");
    printf("\n");

    if (perf_tests[c].teardown != NULL)
      perf_tests[c].teardown();
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
  }

  // End json
  printf("]\n");
  // TODO: close output file
}
