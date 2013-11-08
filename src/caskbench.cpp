#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <popt.h>
#include <err.h>

#include <cairo.h>

#include <SkBitmap.h>
#include <SkBitmapDevice.h>
#include <SkPaint.h>
#include <SkCanvas.h>

#include "caskbench.h"

typedef struct _caskbench_options {
  int dry_run;
  int iterations;
} caskbench_options_t;

typedef struct _caskbench_perf_test {
  const char *name;
  int (*setup)(caskbench_context_t*);
  void (*teardown)(void);
  int (*test_case)(caskbench_context_t*);
} caskbench_perf_test_t;

typedef struct _caskbench_result {
  const char *test_case_name;
  int iterations;
  double min_run_time;
  double avg_run_time;
  int status;
} caskbench_result_t;

caskbench_perf_test_t perf_tests[] = {
  {"cairo-fill",   ca_setup_fill,   ca_teardown_fill,   ca_test_fill},
  {"cairo-image",  ca_setup_image,  ca_teardown_image,  ca_test_image},
  {"cairo-mask",   ca_setup_mask,   ca_teardown_mask,   ca_test_mask},
  {"cairo-paint",  ca_setup_paint,  ca_teardown_paint,  ca_test_paint},
  {"cairo-stroke", ca_setup_stroke, ca_teardown_stroke, ca_test_stroke},

  {"skia-fill",   sk_setup_fill,   sk_teardown_fill,   sk_test_fill},
  {"skia-image",  sk_setup_image,  sk_teardown_image,  sk_test_image},
  {"skia-mask",   sk_setup_mask,   sk_teardown_mask,   sk_test_mask},
  {"skia-paint",  sk_setup_paint,  sk_teardown_paint,  sk_test_paint},
  {"skia-stroke", sk_setup_stroke, sk_teardown_stroke, sk_test_stroke},
};
#define NUM_ELEM(x) (sizeof(x)/sizeof(x[0]))

typedef enum {
  CASKBENCH_STATUS_PASS,
  CASKBENCH_STATUS_FAIL,
  CASKBENCH_STATUS_ERROR,
  CASKBENCH_STATUS_CRASH,
} caskbench_status_t;

static const char *
_status_to_string(int result)
{
  switch (result) {
  case CASKBENCH_STATUS_PASS:
    return "PASS";
  case CASKBENCH_STATUS_FAIL:
    return "FAIL";
  case CASKBENCH_STATUS_ERROR:
    return "ERROR";
  case CASKBENCH_STATUS_CRASH:
    return "CRASH";
  default:
    return "unknown";
  }
}

void
process_options(caskbench_options_t *opt, int argc, char *argv[])
{
  int rc;
  poptContext pc;
  struct poptOption po[] = {
    {"iterations", 'i', POPT_ARG_INT, &opt->iterations, 0,
     "The number of times each test case should be run",
     NULL},
    {"dry-run", 'n', POPT_ARG_NONE, &opt->dry_run, 0,
     "Just list the selected test case names without executing",
     NULL},
    POPT_AUTOHELP
    {NULL}
  };

  assert(opt);

  // Initialize options
  opt->dry_run = 0;
  opt->iterations = 64;

  pc = poptGetContext(NULL, argc, (const char **)argv, po, 0);
  poptSetOtherOptionHelp(pc, "[ARG...]");
  poptReadDefaultConfig(pc, 0);

  while ((rc = poptGetNextOpt(pc)) >= 0) {
    printf("%d\n", rc);
  }
  if (rc != -1) {
    // handle error
    switch(rc) {
    case POPT_ERROR_NOARG:
      errx(1, "Argument missing for an option\n");
    case POPT_ERROR_BADOPT:
      errx(1, "Unknown option or argument\n");
    case POPT_ERROR_BADNUMBER:
    case POPT_ERROR_OVERFLOW:
      errx(1, "Option could not be converted to number\n");
    default:
      errx(1, "Unknown error in option processing\n");
    }
  }

  // Remaining args are the tests to be run, or all if this list is empty
  const char **tests = poptGetArgs(pc);
}

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
  printf("       \"status\": \"%s\"\n", _status_to_string(result->status));
  printf("   }");
}

int
main (int argc, char *argv[])
{
  int c, i;
  caskbench_options_t opt;
  double start_time, stop_time, run_time, run_total;

  process_options(&opt, argc, argv);

  // TODO: Open output file
  // start json
  printf("[\n");
  for (c=0; c<NUM_ELEM(perf_tests); c++) {
    // Setup
    caskbench_context_t context;
    caskbench_result_t result;
    cairo_surface_t *cairo_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 800, 80);

    SkBitmap bitmap;
    bitmap.setConfig(SkBitmap::kARGB_8888_Config, 800, 100);
    bitmap.allocPixels();
    SkBitmapDevice device(bitmap);
    SkCanvas canvas(&device);
    SkPaint paint;

    // If dry run, just list the test cases
    if (opt.dry_run) {
      printf("%s\n", perf_tests[c].name);
      continue;
    }

    srand(0xdeadbeef);
    context.cr = cairo_create(cairo_surface);
    context.paint = &paint;
    context.canvas = &canvas;

    result.test_case_name = perf_tests[c].name;
    result.min_run_time = -1.0;
    result.avg_run_time = -1.0;

    if (perf_tests[c].setup != NULL)
      if (!perf_tests[c].setup(&context)) {
	result.status = CASKBENCH_STATUS_ERROR;
	goto FINAL;
      }

    // Run once to warm caches and calibrate
    perf_tests[c].test_case(&context);

    run_total = 0;
    for (i=opt.iterations; i>0; --i) {
      try {
	assert(perf_tests[c].test_case);
	start_time = get_tick();

	// Execute test_case
	if (perf_tests[c].test_case(&context))
	  result.status = CASKBENCH_STATUS_PASS;
	else
	  result.status = CASKBENCH_STATUS_FAIL;

	stop_time = get_tick();
	run_time = stop_time - start_time;
	if (result.min_run_time < 0)
	  result.min_run_time = run_time;
	else
	  result.min_run_time = MIN(run_time, result.min_run_time);
	run_total += run_time;
      } catch (...) {
	warnx("Unknown exception encountered\n");
	result.status = CASKBENCH_STATUS_CRASH;
	goto FINAL;
      }
    }
    result.iterations = opt.iterations - i;
    result.avg_run_time = run_total / (double)result.iterations;

    {
      SkAutoLockPixels image_lock(bitmap);
      cairo_surface_t* skia_surface = cairo_image_surface_create_for_data((unsigned char*)bitmap.getPixels(),
									  CAIRO_FORMAT_ARGB32,
									  bitmap.width(), bitmap.height(), 
									  bitmap.rowBytes());

      cairo_surface_write_to_png (skia_surface,  "fill-skia.png");
      cairo_surface_destroy(skia_surface);
    }
 
    cairo_surface_write_to_png (cairo_surface, "fill-cairo.png");

  FINAL:
    // TODO: Print results to stdout plain text
    display_results_json(&result);
    if (c != NUM_ELEM(perf_tests)-1)
      printf(",");
    printf("\n");

    if (perf_tests[c].teardown != NULL)
      perf_tests[c].teardown();

    cairo_destroy(context.cr);
    cairo_surface_destroy(cairo_surface);
  }

  // End json
  printf("]\n");
  // TODO: close output file
}
