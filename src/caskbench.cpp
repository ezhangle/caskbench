#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <popt.h>
#include <err.h>

#include <cairo.h>
#include <cairo-gl.h>

#include <SkBitmap.h>
#include <SkBitmapDevice.h>
#include <SkPaint.h>
#include <SkCanvas.h>

#include "caskbench.h"

typedef struct _caskbench_options {
  int dry_run;
  int iterations;
  int size;
  char* output_file;
  char* surface_type;
} caskbench_options_t;

typedef struct _caskbench_perf_test {
  const char *name;
  int (*setup)(caskbench_context_t*);
  void (*teardown)(void);
  int (*test_case)(caskbench_context_t*);
  void (*write_image)(const char *, caskbench_context_t*);
} caskbench_perf_test_t;

typedef struct _caskbench_result {
  const char *test_case_name;
  int size;
  int iterations;
  double min_run_time;
  double avg_run_time;
  int status;
} caskbench_result_t;

// Surfaces
cairo_surface_t *
create_cairo_surface_glx (int width, int height);

cairo_surface_t *
create_cairo_surface_egl (int width, int height);


void
write_image_file_cairo (const char *fname, caskbench_context_t *context)
{
  assert (context->cairo_surface);
  cairo_surface_write_to_png (context->cairo_surface, fname);
}


void
write_image_file_skia (const char *fname, caskbench_context_t *context)
{
  SkBaseDevice *device = context->skia_device;
  assert (device);

  const SkBitmap &bitmap = device->accessBitmap(false);
  SkAutoLockPixels image_lock(bitmap);
  cairo_surface_t* surface = cairo_image_surface_create_for_data((unsigned char*)bitmap.getPixels(),
								 CAIRO_FORMAT_ARGB32,
								 bitmap.width(), bitmap.height(),
								 bitmap.rowBytes());

  cairo_surface_write_to_png (surface, fname);
  cairo_surface_destroy(surface);
}


caskbench_perf_test_t perf_tests[] = {
  {"cairo-fill",   ca_setup_fill,   ca_teardown_fill,   ca_test_fill,   write_image_file_cairo},
  {"skia-fill",    sk_setup_fill,   sk_teardown_fill,   sk_test_fill,   write_image_file_skia },

  {"cairo-image",  ca_setup_image,  ca_teardown_image,  ca_test_image,   write_image_file_cairo},
  {"skia-image",   sk_setup_image,  sk_teardown_image,  sk_test_image,   write_image_file_skia },

  {"cairo-mask",   ca_setup_mask,   ca_teardown_mask,   ca_test_mask,   write_image_file_cairo},
  {"skia-mask",    sk_setup_mask,   sk_teardown_mask,   sk_test_mask,   write_image_file_skia },

  {"cairo-paint",  ca_setup_paint,  ca_teardown_paint,  ca_test_paint,   write_image_file_cairo},
  {"skia-paint",   sk_setup_paint,  sk_teardown_paint,  sk_test_paint,   write_image_file_skia },

  {"cairo-stroke", ca_setup_stroke, ca_teardown_stroke, ca_test_stroke,   write_image_file_cairo},
  {"skia-stroke",  sk_setup_stroke, sk_teardown_stroke, sk_test_stroke,   write_image_file_skia },

  {"cairo-rectangles", ca_setup_rectangles, ca_teardown_rectangles, ca_test_rectangles,   write_image_file_cairo},
  {"skia-rectangles",  sk_setup_rectangles, sk_teardown_rectangles, sk_test_rectangles,   write_image_file_skia },

  {"cairo-roundrect", ca_setup_roundrect, ca_teardown_roundrect, ca_test_roundrect,       write_image_file_cairo},
  {"skia-roundrect",  sk_setup_roundrect, sk_teardown_roundrect, sk_test_roundrect,       write_image_file_skia },

  {"cairo-bubbles", ca_setup_bubbles, ca_teardown_bubbles, ca_test_bubbles,               write_image_file_cairo},
  {"skia-bubbles",  sk_setup_bubbles, sk_teardown_bubbles, sk_test_bubbles,               write_image_file_skia },

  {"cairo-multishape", ca_setup_multishape, ca_teardown_multishape, ca_test_multishape,   write_image_file_cairo},
  {"skia-multishape",  sk_setup_multishape, sk_teardown_multishape, sk_test_multishape,   write_image_file_skia },
};

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
    {"output-file", 'o', POPT_ARG_STRING, &opt->output_file, 0,
     "Filename to write JSON output to",
     NULL},
    {"test-size", 's', POPT_ARG_INT, &opt->size, 0,
     "Controls the complexity of the tests, such as number of drawn elements",
     NULL},
    {"surface-type", 't', POPT_ARG_STRING, &opt->surface_type, 0,
     "Type of graphics surface to use (image, glx, or egl)",
     NULL},
    POPT_AUTOHELP
    {NULL}
  };

  // Initialize options
  opt->dry_run = 0;
  opt->iterations = 64;
  opt->size = 64;
  opt->output_file = NULL;
  opt->surface_type = NULL;

  // Process the command line
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
  //const char **tests = poptGetArgs(pc);
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

int
main (int argc, char *argv[])
{
  int c, i;
  caskbench_options_t opt;
  double start_time, stop_time, run_time, run_total;
  double cairo_avg_time = 0.0;
  double perf_improvement = 0.0;
  FILE *fp;
  char fname[256];

  process_options(&opt, argc, argv);

  if (opt.output_file) {
    // start writing json to output file
    fp = fopen(opt.output_file, "w");
    fprintf(fp, "[\n");
  }
  for (c=0; c<NUM_ELEM(perf_tests); c++) {
    caskbench_context_t context;
    caskbench_result_t result;
    cairo_surface_t *cairo_surface;
    SkBitmap skia_bitmap;
    SkPaint skia_paint;

    context.size = opt.size;
    context.canvas_width = 800;
    context.canvas_height = 400;

    if (opt.surface_type == NULL || !strncmp(opt.surface_type, "image", 5)) {
      cairo_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
						  context.canvas_width,
						  context.canvas_height);
    } else if (!strncmp(opt.surface_type, "glx", 3)) {
      cairo_surface = create_cairo_surface_glx ( context.canvas_width,
						  context.canvas_height);
    } else if (!strncmp(opt.surface_type, "egl", 3)) {
      cairo_surface = create_cairo_surface_egl ( context.canvas_width,
						  context.canvas_height);
    }

    if (!cairo_surface)
      errx(2, "Could not create a cairo surface\n");

    skia_bitmap.setConfig(SkBitmap::kARGB_8888_Config,
			  context.canvas_width, context.canvas_height);
    skia_bitmap.allocPixels();
    SkBitmapDevice skia_device(skia_bitmap);
    SkCanvas skia_canvas(&skia_device);

    // If dry run, just list the test cases
    if (opt.dry_run) {
      printf("%s\n", perf_tests[c].name);
      continue;
    }

    srand(0xdeadbeef);
    context.cr = cairo_create(cairo_surface);
    context.paint = &skia_paint;
    context.canvas = &skia_canvas;
    context.cairo_surface = cairo_surface;
    context.skia_device = &skia_device;

    result.test_case_name = perf_tests[c].name;
    result.size = 0;
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
    result.size = opt.size;

    // Write image to file
    if (perf_tests[c].write_image) {
      snprintf(fname, sizeof(fname), "%s.png", perf_tests[c].name);
      perf_tests[c].write_image (fname, &context);
    }

  FINAL:
    printf("%-20s %-4d   %s  %d  %f  %f",
	   result.test_case_name,
	   result.size,
	   _status_to_string(result.status),
	   result.iterations,
	   result.min_run_time,
	   result.avg_run_time);

    if (result.test_case_name[0] == 'c') {
      perf_improvement = 0.0;
      cairo_avg_time = result.avg_run_time;
    } else {
      perf_improvement = (cairo_avg_time - result.avg_run_time)/cairo_avg_time;
      cairo_avg_time = 0.0;
      printf("  %4.2f%%", perf_improvement * 100.0);
    }
    printf("\n");

    if (opt.output_file) {
      fprintf(fp, "   {\n");
      fprintf(fp, "       \"test case\": \"%s\",\n", result.test_case_name);
      fprintf(fp, "       \"size\": \"%d\",\n", result.size);
      fprintf(fp, "       \"status\": \"%s\"\n", _status_to_string(result.status));
      fprintf(fp, "       \"iterations\": %d,\n", result.iterations);
      fprintf(fp, "       \"minimum run time (s)\": %f,\n", result.min_run_time);
      fprintf(fp, "       \"average run time (s)\": %f,\n", result.avg_run_time);
      fprintf(fp, "   }");

      if (c != NUM_ELEM(perf_tests)-1)
	fprintf(fp, ",");
      fprintf(fp, "\n");
    }

    if (perf_tests[c].teardown != NULL)
      perf_tests[c].teardown();

    cairo_destroy(context.cr);
    cairo_surface_destroy(cairo_surface);
  }

  if (opt.output_file) {
    // End json
    fprintf(fp, "]\n");
    fclose(fp);
  }
}
