/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <popt.h>
#include <err.h>

#include <cairo.h>
#ifdef HAVE_CAIRO_GL_H
#  include <cairo-gl.h>
#endif

#ifdef USE_SKIA
#  include <SkBitmap.h>
#  include <SkBitmapDevice.h>
#  include <SkPaint.h>
#  include <SkCanvas.h>
#endif

#include "caskbench.h"
#include "device_config.h"
#include "tests.h"

#ifndef SkToS32
int32_t SkToS32(intmax_t x) { return (int32_t)x; }
#endif

bool gPrintInstCount = false;

typedef struct _caskbench_options {
    int dry_run;
    int iterations;
    int list_surfaces;
    char* output_file;
    char* surface_type;
    int size;
    int version;

    unsigned int enable_egl_sample_buffers;
    char* shape_name;
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
    char *stock_image_path;
    int stroke_width;
    int multi_shapes;

    double stroke_red;
    double stroke_green;
    double stroke_blue;

    int cap_style;
    int join_style;
    int dash_style;
    char* seed_value;
    char* tests;
    unsigned int enable_output_images;
    double tolerance;
    int canvas_width;
    int canvas_height;
} caskbench_options_t;

typedef struct _caskbench_result {
    const char *test_case_name;
    int size;
    int iterations;
    double min_run_time;
    double avg_run_time;
    double max_run_time;
    double median_run_time;
    double standard_deviation;
    double avg_frames_per_second;
    int status;
} caskbench_result_t;

const char *gShapes[] = {
    "circle",
    "rectangle",
    "triangle",
    "star",
    "roundedrectangle",
    NULL
};

const char *gFillTypes[] = {
    "none",
    "solid",
    "linear-gradient",
    "radial-gradient",
    "image-pattern",
    "herringbone-pattern",
    NULL
};

typedef enum {
    CB_STATUS_PASS,
    CB_STATUS_FAIL,
    CB_STATUS_ERROR,
    CB_STATUS_CRASH,
} caskbench_status_t;

static const char *
_status_to_string(int result)
{
    switch (result) {
    case CB_STATUS_PASS:
        return "PASS";
    case CB_STATUS_FAIL:
        return "FAIL";
    case CB_STATUS_ERROR:
        return "ERROR";
    case CB_STATUS_CRASH:
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
        {"shape", 'S', POPT_ARG_STRING, &opt->shape_name, 0,
         "Controls which shape to be drawn ",
         NULL},
        {"x-position", 'X', POPT_ARG_INT, &opt->x_position, 0,
         "The X location to draw the object",
         NULL},
        {"y-position", 'Y', POPT_ARG_INT, &opt->y_position, 0,
         "The Y location to draw the object",
         NULL},
        {"object-width", '\0', POPT_ARG_INT, &opt->width, 0,
         "Width of the shape bject ",
         NULL},
        {"object-height", '\0', POPT_ARG_INT, &opt->height, 0,
         "Height of the shape object ",
         NULL},
        {"fill-type", 'f', POPT_ARG_STRING, &opt->fill_type, 0,
         "Controls the fill type of the objects draw either solid, gradient, image pattern type",
         NULL},
#if 0
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
#endif
        {"animation", 'g', POPT_ARG_INT, &opt->animation, 0,
         "Controls the kinematics of the objects drawn",
         NULL},
        {"image-path", 'I', POPT_ARG_STRING, &opt->stock_image_path, 0,
         "Path to a stock image for use in clipping, patterns, etc.",
         NULL},
        {"stroke-width", '\0', POPT_ARG_INT, &opt->stroke_width, 0,
         "represents stroke width of the object",
         NULL},
        // TODO: Crashes
        {"multi-shapes", 'm', POPT_ARG_INT, &opt->multi_shapes, 0,
         "represents stroke width of the object",
         NULL},
#if 0
        {"stroke-red", "SR", POPT_ARG_INT, &opt->multi_shapes, 0,
         "represents r value for stroke color",
         NULL},
        {"stroke-green", "SG", POPT_ARG_INT, &opt->multi_shapes, 0,
         "represents represents g value for stroke color",
         NULL},
        {"stroke-blue", "SB", POPT_ARG_INT, &opt->multi_shapes, 0,
         "represents represents b value for stroke color",
         NULL},
#endif
#if 0
        {"cap-style", 'C', POPT_ARG_INT, &opt->cap_style, 0,
         "represents r value for stroke color",
         NULL},
        {"join-style", 'J', POPT_ARG_INT, &opt->join_style, 0,
         "represents r value for stroke color",
         NULL},
        {"dash-style", 'D', POPT_ARG_INT, &opt->dash_style, 0,
         "represents r value for stroke color",
         NULL},
#endif
        {"seed-value", 'r', POPT_ARG_STRING, &opt->seed_value, 0,
         "Represents seed value for Random Number generater eg. -r ABCDEFFF",
         NULL},
        {"tests", 'T', POPT_ARG_STRING, &opt->tests, 0,
         "Controls list of tests to run eg. -T \"bubbles roundrect ...\" " ,
         NULL},
        {"enable-output-images", '\0', POPT_ARG_NONE, &opt->enable_output_images, 0,
         "Generate image PNG files displaying the final rendering frame from each test run.",
         NULL},
        {"tolerance", '\0', POPT_ARG_DOUBLE, &opt->tolerance, 0,
         "Sets tesselation tolerance value for cairo",
         NULL},
        {"canvas_width", 'w', POPT_ARG_INT, &opt->canvas_width, 0,
         "Width of canvas used for each test case (defaults to 800)",
         NULL},
        {"canvas_height", 'h', POPT_ARG_INT, &opt->canvas_height, 0,
         "Height of canvas used for each test case (defaults to 400)",
         NULL},
        POPT_AUTOHELP
        {NULL}
    };

    // Initialize options
    memset (opt, 0, sizeof(caskbench_options_t));
    opt->iterations = 10;
    opt->size = 64;

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

shape_type_t
convertToShapeType(const char* shape_name)
{
    int i =0;
    if (shape_name == NULL)
        return (shape_type_t) 0;
    while (gShapes[i] != NULL) {
        if (strcmp(gShapes[i], shape_name) == 0)
            return (shape_type_t)(i + 1);
        i++;
    }
    return (shape_type_t) 0;
}

fill_type_t
convertToFillType(const char *fill_name)
{
    int i =0;
    if (fill_name == NULL)
        return (fill_type_t) 0;
    while (gFillTypes[i] != NULL) {
        if (strcmp(gFillTypes[i], fill_name) == 0)
            return (fill_type_t) i ;
        i++;
    }
    return (fill_type_t) 0;
}

void
shape_defaults_init(shapes *shape_defaults, caskbench_options_t *opt)
{
    shape_defaults->x = opt->x_position;
    shape_defaults->y = opt->y_position;
    shape_defaults->width = opt->width;
    shape_defaults->height = opt->height;
    shape_defaults->shape_type = convertToShapeType(opt->shape_name);
    shape_defaults->fill_type = convertToFillType(opt->fill_type);
    shape_defaults->red = opt->red;
    shape_defaults->green = opt->green;
    shape_defaults->blue = opt->blue;
    shape_defaults->alpha = opt->alpha;
    shape_defaults->stroke_width = opt->stroke_width;
    shape_defaults->multi_shapes = opt->multi_shapes;
    shape_defaults->animation = opt->animation;
    shape_defaults->stroke_width = opt->stroke_width;
    shape_defaults->stroke_red = opt->stroke_red;
    shape_defaults->stroke_green = opt->stroke_green;
    shape_defaults->stroke_blue = opt->stroke_blue;
    shape_defaults->cap_style = opt->cap_style;
    shape_defaults->join_style = opt->join_style;
    shape_defaults->dash_style = opt->dash_style;
    shape_defaults->radius = 0.0;
}

void
result_init(caskbench_result_t *result, const char* name)
{
    assert(result);

    result->test_case_name = name;
    result->size = 0;
    result->min_run_time = -1.0;
    result->avg_run_time = -1.0;
    result->max_run_time = -1.0;
    result->median_run_time = -1.0;
    result->standard_deviation = -1.0;
    result->avg_frames_per_second = -1.0;
}

double median_run_time (double data[], int n)
{
    double temp;
    int i, j;
    for (i = 0; i < n; i++)
        for (j = i+1; j < n; j++)
        {
            if (data[i] > data[j])
            {
                temp = data[j];
                data[j] = data[i];
                data[i] = temp;
            }
        }
    if (n % 2 == 0)
        return (data[n/2] + data[n/2-1])/2;
    else
        return data[n/2];
}

double standard_deviation (const double data[], int n, double mean)
{
    double sum_deviation = 0.0;
    int i;
    for (i = 0; i < n; ++i)
    sum_deviation += (data[i]-mean) * (data[i]-mean);
    return sqrt (sum_deviation / n);
}

int
convertToTestId(const char* test_name)
{
    int j = 0;
    if (test_name == NULL)
        return -1;
    /* Checks whether the user provided test case is valid */
    for(j = 0; j < num_perf_tests; j++) {
        if (strstr (perf_tests[j].name, test_name))
            return j;
    }
    return -1;
}

void populate_user_tests (const char* input, int& num_tests, int* test_ids)
{
    int i = 0, test_index = 0, k = 0, id = -1;
    char temp[MAX_BUFFER];
    if(input == NULL)
        return;
    for (i = 0; i <= strlen(input); i++) {
        if ((input[i]!=' ') && (input[i]!='\0')) {
            temp[k++] = input[i];
            if (k >= MAX_BUFFER)
                errx (0, "Please check test case name, it exceeds maximum buffer limit \n");
            continue;
        }
        temp[k] = '\0';
        k = 0;
        /* Check whether the user specified test is present in performance test list */
        id = convertToTestId (temp);
        if (id < 0)
            errx (0, "Incorrect test case, please check the test case provided as input \n");
        /* Add cairo test index to the test list */
        test_ids[test_index++] = id;
#ifdef USE_SKIA
        /* Add skia test index to the test list */
        test_ids[test_index++] = id + 1;
        num_tests = num_tests + 2;
#else
        num_tests = num_tests + 1;
#endif
    }
}

int
main (int argc, char *argv[])
{
    int c, i, num_user_tests = 0, num_tests = 0;
    int user_test_ids[num_perf_tests];
    caskbench_options_t opt;
    double start_time, stop_time, run_time, run_total;
    double cairo_avg_time = 0.0;
    double perf_improvement = 0.0;
    FILE *fp;
    char fname[256], *end_ptr;
    device_config_t config;

    process_options(&opt, argc, argv);
    double run_time_values[opt.iterations];

    memset (&config, 0, sizeof(device_config_t));

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
    memset (user_test_ids, -1, sizeof(user_test_ids));
    populate_user_tests (opt.tests, num_user_tests, user_test_ids);
    num_tests = num_user_tests?num_user_tests:num_perf_tests;
    for(int s = 0; s < num_tests; s++) {
        c = num_user_tests ? user_test_ids[s]:s;
        caskbench_context_t context;
        caskbench_result_t result;

        /* Disable stroke: Displays no output to file */
        if (!strncmp(perf_tests[c].name, "cairo-stroke", 12) ||
            !strncmp(perf_tests[c].name, "skia-stroke", 11))
            continue;

        /* Disable clip:  Test fails */
        if (!strncmp(perf_tests[c].name, "cairo-clip", 10) ||
            !strncmp(perf_tests[c].name, "skia-clip", 9))
            continue;

        if(opt.seed_value == NULL)
        {
            srand (0xdeadbeef);
        }
        else
        {
            srand (strtoul(opt.seed_value,&end_ptr,16));
        }

        context_init(&context, opt.size);
        if(opt.canvas_width)
            context.canvas_width = opt.canvas_width;
        if(opt.canvas_height)
            context.canvas_height = opt.canvas_height;

        shape_defaults_init(&context.shape_defaults, &opt);
        result_init(&result, perf_tests[c].name);
        config.width = context.canvas_width;
        config.height = context.canvas_height;
        config.surface_type = opt.surface_type;

        if(opt.tolerance)
           context.tolerance = opt.tolerance;

        perf_tests[c].context_setup(&context, config);

        if (perf_tests[c].setup != NULL)
            if (!perf_tests[c].setup(&context)) {
                result.status = CB_STATUS_ERROR;
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
                    result.status = CB_STATUS_PASS;
                else
                    result.status = CB_STATUS_FAIL;
                perf_tests[c].context_update(&context);

                stop_time = get_tick();
                run_time = stop_time - start_time;
                run_time_values[opt.iterations-i] = run_time;
                if (result.min_run_time < 0)
                    result.min_run_time = run_time;
                else
                    result.min_run_time = MIN(run_time, result.min_run_time);
                result.max_run_time = MAX(run_time, result.max_run_time);
                run_total += run_time;
            } catch (...) {
                warnx("Unknown exception encountered\n");
                result.status = CB_STATUS_CRASH;
                goto FINAL;
            }
        }
        result.iterations = opt.iterations - i;
        result.avg_run_time = run_total / (double)result.iterations;
        result.size = opt.size;
        result.median_run_time = median_run_time (run_time_values, result.iterations);
        result.standard_deviation = standard_deviation (run_time_values, result.iterations, result.avg_run_time);
        result.avg_frames_per_second = (1.0 / result.avg_run_time);
        result.avg_frames_per_second = (result.avg_frames_per_second<9999) ? result.avg_frames_per_second:9999;

        // Write image to file
        if (opt.enable_output_images && perf_tests[c].write_image) {
            snprintf(fname, sizeof(fname), "%s.png", perf_tests[c].name);
            perf_tests[c].write_image (fname, &context);
        }

    FINAL:
        /* Removed min_run_time, avg_run_time to avoid confusion as min/max/avg run times are captured in json output */
        printf("%-20s %-4d   %s  %d  %4.0f",
               result.test_case_name,
               result.size,
               _status_to_string(result.status),
               result.iterations,
               result.avg_frames_per_second);

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
            fprintf(fp, "       \"average run time (s)\": %f,\n", result.avg_run_time);
            fprintf(fp, "       \"maximum run time (s)\": %f,\n", result.max_run_time);
            fprintf(fp, "       \"median run time (s)\": %f,\n", result.median_run_time);
            fprintf(fp, "       \"standard deviation of run time (s)\": %f,\n", result.standard_deviation);
            fprintf(fp, "       \"avg frames per second (fps)\": %-4.0f\n", result.avg_frames_per_second);
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

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
