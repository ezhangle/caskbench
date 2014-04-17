#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <popt.h>
#include <err.h>

#include <cairo.h>
#ifdef HAVE_CAIRO_GL_H
#  include <cairo-gl.h>
#endif

#include <SkBitmap.h>
#include <SkBitmapDevice.h>
#include <SkPaint.h>
#include <SkCanvas.h>

#include "caskbench.h"
#include "device_config.h"
#include "tests.h"

#ifndef SkToS32
int32_t SkToS32(intmax_t x) { return (int32_t)x; }
#endif

#ifndef USE_LEGACY_SKIA_SRA
bool gPrintInstCount = false;
#endif

typedef struct _caskbench_options {
    int dry_run;
    int iterations;
    int list_surfaces;
    char* output_file;
    char* surface_type;
    int size;
    int version;

    unsigned int enable_egl_sample_buffers;
    int shape_id;
    int x_position;
    int y_position;
    int width;
    int height;
    char* fill_type;
    double red;
    double green;
    double blue;
    double alpha;
    int animation;
    char *image_path;
    int stroke_width;
    int multi_shapes;

    double stroke_red;
    double stroke_green;
    double stroke_blue;

    int cap_style;
    int join_style;
    int dash_style;
} caskbench_options_t;

typedef struct _caskbench_result {
    const char *test_case_name;
    int size;
    int iterations;
    double min_run_time;
    double avg_run_time;
    int status;
} caskbench_result_t;

// Backend-specific graphics initialization
cairo_surface_t * create_cairo_surface_image (const device_config_t& config);
void destroy_cairo_image();
void update_cairo_image();

SkBaseDevice * create_skia_device_image (const device_config_t& config);
void destroy_skia_image();
void update_skia_image();

#if USE_GLX
cairo_surface_t *create_cairo_surface_glx (const device_config_t& config);
void destroy_cairo_glx();
void update_cairo_glx();

SkBaseDevice * create_skia_device_glx (const device_config_t& config);
void destroy_skia_glx();
void update_skia_glx();
#endif

#if USE_EGL
cairo_surface_t * create_cairo_surface_egl (const device_config_t& config);
void destroy_cairo_egl();
void update_cairo_egl();

SkBaseDevice * create_skia_device_egl (const device_config_t& config);
void destroy_skia_egl();
void update_skia_egl();
#endif

void
write_image_file_cairo (const char *fname, caskbench_context_t *context)
{
    assert (context->cairo_surface);
    cairo_status_t status = cairo_surface_status (context->cairo_surface);
    if (status != CAIRO_STATUS_SUCCESS) {
      warnx("Error writing cairo surface to file: %s\n", cairo_status_to_string(status));
      return;
    }
    cairo_surface_write_to_png (context->cairo_surface, fname);
}


void
write_image_file_skia (const char *fname, caskbench_context_t *context)
{
    SkBitmap bitmap;
    void * data;
    cairo_surface_t* surface;

#ifdef USE_LEGACY_SKIA_SRA
    bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                     context->canvas_width, context->canvas_height,
                     4 * context->canvas_width, // TODO: Get from context->skia_device
                     kPremul_SkAlphaType);
#else
    bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                     context->canvas_width, context->canvas_height);
    SkImageInfo info = SkImageInfo::Make(context->canvas_width, context->canvas_height,
                                         kBGRA_8888_SkColorType,
                                         kPremul_SkAlphaType);
    bitmap.allocPixels(info);
#endif
    context->skia_canvas->flush();
    if (!context->skia_canvas->readPixels(&bitmap, 0, 0, SkCanvas::kRGBA_Unpremul_Config8888)) {
        warnx("Could not read pixels from skia device\n");
        return;
    }

    data = bitmap.getPixels();
    assert(data);
    surface = cairo_image_surface_create_for_data((unsigned char*)data,
                                                  CAIRO_FORMAT_ARGB32,
                                                  context->canvas_width, context->canvas_height,
                                                  4 * context->canvas_width);

    cairo_status_t status = cairo_surface_status (surface);
    if (status != CAIRO_STATUS_SUCCESS) {
      warnx("Error writing skia surface to file: %s\n", cairo_status_to_string(status));
      return;
    }
    cairo_surface_write_to_png (surface, fname);
    cairo_surface_destroy(surface);
}


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

static void
print_version()
{
    printf("%s version %s\n",
           PACKAGE_NAME,
           PACKAGE_VERSION);
}

static void
print_surfaces_available()
{
    printf("image\n");
#if USE_GLX
    printf("glx\n");
#endif
#if USE_EGL
    printf("egl\n");
#endif
}

void
process_options(caskbench_options_t *opt, int argc, char *argv[])
{
    int rc;
    poptContext pc;
    struct poptOption po[] = {
        {"dry-run", 'n', POPT_ARG_NONE, &opt->dry_run, 0,
         "Just list the selected test case names without executing",
         NULL},
        {"iterations", 'i', POPT_ARG_INT, &opt->iterations, 0,
         "The number of times each test case should be run",
         NULL},
        {"list-surfaces", 'l', POPT_ARG_NONE, &opt->list_surfaces, 0,
         "List the available surfaces linked in this executable",
         NULL},
        {"output-file", 'o', POPT_ARG_STRING, &opt->output_file, 0,
         "Filename to write JSON output to",
         NULL},
        {"surface-type", 't', POPT_ARG_STRING, &opt->surface_type, 0,
         "Type of graphics surface to use (see --list-surfaces for available surfaces)",
         NULL},
        {"test-size", 's', POPT_ARG_INT, &opt->size, 0,
         "Controls the complexity of the tests, such as number of drawn elements",
         NULL},
        {"version", 'V', POPT_ARG_NONE, &opt->version, 0,
         "Display the program version",
         NULL},
        {"enable-egl-sample-buffers", '\0', POPT_ARG_NONE, &opt->enable_egl_sample_buffers, 0,
         "Sets EGL_SAMPLES=4 and EGL_SAMPLE_BUFFERS=1 in the EGL attribute list",
         NULL},
        {"shape_id", 'S', POPT_ARG_INT, &opt->shape_id, 0,
         "Controls which shape to be drawn ",
         NULL},
        {"x_position", 'X', POPT_ARG_INT, &opt->x_position, 0,
         "x_position of the object where it to be drawn ",
         NULL},
        {"y_position", 'Y', POPT_ARG_INT, &opt->y_position, 0,
         "y_position of the object where it to be drawn ",
         NULL},
        {"width", 'W', POPT_ARG_INT, &opt->width, 0,
         "Width of the shape bject ",
         NULL},
        {"height", 'H', POPT_ARG_INT, &opt->height, 0,
         "Height of the shape object ",
         NULL},
        {"fill-type", 'f', POPT_ARG_STRING, &opt->fill_type, 0,
         "Controls the fill type of the objects draw either solid, gradient, image pattern type",
         NULL},
        {"red", 'R', POPT_ARG_DOUBLE, &opt->red, 0,
         "R Color Value",
         NULL},
        {"green", 'G', POPT_ARG_DOUBLE, &opt->green, 0,
         "g Color Value",
         NULL},
        {"blue", 'B', POPT_ARG_DOUBLE, &opt->blue, 0,
         "B Color Value",
         NULL},
        {"alpha", 'A', POPT_ARG_DOUBLE, &opt->alpha, 0,
         "Transparency value for the solid fill",
         NULL},
        {"animation", 'g', POPT_ARG_INT, &opt->animation, 0,
         "Controls the kinematics of the objects drawn",
         NULL},
        {"image-path", 'I', POPT_ARG_STRING, &opt->image_path, 0,
         "Image path to refer the image for setting source, clipping, patterns",
         NULL},
        {"stroke_width", 'w', POPT_ARG_INT, &opt->stroke_width, 0,
         "represents stroke width of the object",
         NULL},
        {"multi_shapes", 'm', POPT_ARG_INT, &opt->multi_shapes, 0,
         "represents stroke width of the object",
         NULL},
#if 0
        {"stroke_red", "SR", POPT_ARG_INT, &opt->multi_shapes, 0,
         "represents r value for stroke color",
         NULL},
        {"stroke_green", "SG", POPT_ARG_INT, &opt->multi_shapes, 0,
         "represents represents g value for stroke color",
         NULL},
        {"stroke_blue", "SB", POPT_ARG_INT, &opt->multi_shapes, 0,
         "represents represents b value for stroke color",
         NULL},
#endif
        {"cap_style", 'C', POPT_ARG_INT, &opt->cap_style, 0,
         "represents r value for stroke color",
         NULL},
        {"join_style", 'J', POPT_ARG_INT, &opt->join_style, 0,
         "represents r value for stroke color",
         NULL},
        {"dash_style", 'D', POPT_ARG_INT, &opt->dash_style, 0,
         "represents r value for stroke color",
         NULL},
        POPT_AUTOHELP
        {NULL}
    };

    // Initialize options
    opt->dry_run = 0;
    opt->iterations = 10;
    opt->list_surfaces = 0;
    opt->output_file = NULL;
    opt->size = 64;
    opt->surface_type = NULL;
    opt->version = 0;


    opt->enable_egl_sample_buffers = 0;
    opt->shape_id = 0;
    opt->x_position = 0;
    opt->y_position = 0;
    opt->width = 0;
    opt->height = 0;
    opt->fill_type = NULL;
    opt->red = 0;
    opt->green = 0;
    opt->blue = 0;
    opt->alpha = 0;
    opt->animation = 0;
    opt->image_path = NULL;
    opt->multi_shapes = 0;
    opt->stroke_width = 0;
    opt->stroke_red = 0;
    opt->stroke_green = 0;
    opt->stroke_blue = 0;
    opt->cap_style = 0;
    opt->join_style = 0;
    opt->dash_style = 0;

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

void
context_init(caskbench_context_t *context, int size)
{
    assert(context);
    assert(size > 0);

    context->size = size;
    context->canvas_width = 800;
    context->canvas_height = 400;
    context->skia_device = NULL;
    context->skia_paint = NULL;

    context->setup_cairo = NULL;
    context->setup_skia = NULL;
    context->destroy_cairo = NULL;
    context->destroy_skia = NULL;
    context->update_cairo = NULL;
    context->update_skia = NULL;
}

void
context_setup_cairo(caskbench_context_t *context, const device_config_t& config)
{
    if (config.surface_type == NULL || !strncmp(config.surface_type, "image", 5)) {
        context->setup_cairo = create_cairo_surface_image;
        context->destroy_cairo = destroy_cairo_image;
        context->update_cairo = update_cairo_image;
    } else if (!strncmp(config.surface_type, "glx", 3)) {
#if USE_GLX
        context->setup_cairo = create_cairo_surface_glx;
        context->destroy_cairo = destroy_cairo_glx;
        context->update_cairo = update_cairo_glx;
#else
        errx(1, "glx unsupported in this build\n");
#endif

    } else if (!strncmp(config.surface_type, "egl", 3)) {
#if USE_EGL
        context->setup_cairo = create_cairo_surface_egl;
        context->destroy_cairo = destroy_cairo_egl;
        context->update_cairo = update_cairo_egl;
#else
        errx(1, "egl unsupported in this build\n");
#endif
    }
    assert (context->setup_cairo);
    assert (context->destroy_cairo);
    assert (context->update_cairo);

    context->cairo_surface = context->setup_cairo(config);
    if (!context->cairo_surface)
        errx(2, "Could not create a cairo surface\n");

    context->cairo_cr = cairo_create(context->cairo_surface);

    // Clear background to black
    cairo_set_source_rgb (context->cairo_cr, 0, 0, 0);
    cairo_paint (context->cairo_cr);

    // Enable anti-aliasing
    cairo_set_antialias (context->cairo_cr, CAIRO_ANTIALIAS_DEFAULT);

    // Ease up Cairo's tessellation tolerance (default is 0.001)
    cairo_set_tolerance (context->cairo_cr, 0.25);
}

void
context_setup_skia(caskbench_context_t *context, const device_config_t& config)
{
    if (config.surface_type == NULL || !strncmp(config.surface_type, "image", 5)) {
        context->setup_skia = create_skia_device_image;
        context->destroy_skia = destroy_skia_image;
        context->update_skia = update_skia_image;
    } else if (!strncmp(config.surface_type, "glx", 3)) {
#if USE_GLX
        context->setup_skia = create_skia_device_glx;
        context->destroy_skia = destroy_skia_glx;
        context->update_skia = update_skia_glx;
#else
        errx(1, "glx unsupported in this build\n");
#endif

    } else if (!strncmp(config.surface_type, "egl", 3)) {
#if USE_EGL
        context->setup_skia = create_skia_device_egl;
        context->destroy_skia = destroy_skia_egl;
        context->update_skia = update_skia_egl;
#else
        errx(1, "egl unsupported in this build\n");
#endif
    }

    assert (context->setup_skia);
    assert (context->destroy_skia);
    assert (context->update_skia);

    context->skia_device = context->setup_skia(config);
    if (!context->skia_device)
        errx(2, "Could not create a skia device\n");

    context->skia_paint = new SkPaint;
    context->skia_canvas = new SkCanvas(context->skia_device);

    // Clear background to black
    context->skia_canvas->clear(0);
    context->skia_paint->setARGB(255, 0, 0, 0);
    context->skia_canvas->drawPaint(*context->skia_paint);

    // Enable anti-aliasing
    context->skia_paint->setAntiAlias(true);
}

void
context_update_cairo(caskbench_context_t *context)
{
    context->update_cairo();
}

void
context_update_skia(caskbench_context_t *context)
{
    context->update_skia();
}

void
context_destroy_skia(caskbench_context_t *context)
{
    if (!context)
        return;
    delete context->skia_paint;
    delete context->skia_canvas;
    delete context->skia_device;
    context->destroy_skia();
    context->skia_device = NULL;
}

void
context_destroy_cairo(caskbench_context_t *context)
{
    if (!context)
        return;
    cairo_destroy(context->cairo_cr);
    cairo_surface_destroy(context->cairo_surface);
    context->destroy_cairo();
    context->cairo_cr = NULL;
}

void
result_init(caskbench_result_t *result, const char* name)
{
    assert(result);

    result->test_case_name = name;
    result->size = 0;
    result->min_run_time = -1.0;
    result->avg_run_time = -1.0;
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
    device_config_t config;

    process_options(&opt, argc, argv);

    config.width = 0;
    config.height = 0;
    config.egl_samples = 0;
    config.egl_sample_buffers = 0;

    if (opt.version) {
        print_version();
        exit(0);
    }
    if (opt.list_surfaces) {
        print_surfaces_available();
        exit(0);
    }
    if (opt.output_file) {
        // start writing json to output file
        fp = fopen(opt.output_file, "w");
        fprintf(fp, "[\n");
    }
    if (opt.enable_egl_sample_buffers) {
        config.egl_samples = 4;
        config.egl_sample_buffers = 1;
    }
    for (c=0; c<num_perf_tests; c++) {
        caskbench_context_t context;
        caskbench_result_t result;

        srand(0xdeadbeef);
        context_init(&context, opt.size);

//Command line setup for shapes
        context.shape_args.centre_x = opt.x_position;
        context.shape_args.centre_y = opt.y_position;
        context.shape_args.width = opt.width;
        context.shape_args.height = opt.height;
        context.shape_args.shape_id = opt.shape_id;
        context.shape_args.fill_type = opt.fill_type;
        context.shape_args.red = opt.red;
        context.shape_args.green = opt.green;
        context.shape_args.blue = opt.blue;
        context.shape_args.alpha = opt.alpha;
        context.shape_args.image_path = opt.image_path;
        context.shape_args.stroke_width = opt.stroke_width;
        context.shape_args.multi_shapes = opt.multi_shapes;
        context.shape_args.animation = opt.animation;
        context.shape_args.stroke_width = opt.stroke_width;
        context.shape_args.stroke_red = opt.stroke_red;
        context.shape_args.stroke_green = opt.stroke_green;
        context.shape_args.stroke_blue = opt.stroke_blue;
        context.shape_args.cap_style = opt.cap_style;
        context.shape_args.join_style = opt.join_style;
        context.shape_args.dash_style = opt.dash_style;

        result_init(&result, perf_tests[c].name);
        config.width = context.canvas_width;
        config.height = context.canvas_height;
        config.surface_type = opt.surface_type;

        perf_tests[c].context_setup(&context, config);

        if (perf_tests[c].setup != NULL)
            if (!perf_tests[c].setup(&context)) {
                result.status = CASKBENCH_STATUS_ERROR;
                goto FINAL;
            }

        // If dry run, just list the test cases
        if (opt.dry_run) {
            printf("%s\n", perf_tests[c].name);
            continue;
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
                perf_tests[c].context_update(&context);

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
            fprintf(fp, "       \"status\": \"%s\",\n", _status_to_string(result.status));
            fprintf(fp, "       \"iterations\": %d,\n", result.iterations);
            fprintf(fp, "       \"minimum run time (s)\": %f,\n", result.min_run_time);
            fprintf(fp, "       \"average run time (s)\": %f\n", result.avg_run_time);
            fprintf(fp, "   }");

            if (c != num_perf_tests-1)
                fprintf(fp, ",");
            fprintf(fp, "\n");
        }

        if (perf_tests[c].teardown != NULL)
            perf_tests[c].teardown();

        perf_tests[c].context_destroy(&context);
    }

    if (opt.output_file) {
        // End json
        fprintf(fp, "]\n");
        fclose(fp);
    }
}
