/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <popt.h>
#include <err.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "caskbench_result.h"
#include "device_config.h"
#include "tests.h"
#include "git_info.h"

typedef struct _caskbench_options {
    int dry_run;
    int iterations;
    int json_output;
    int list_surfaces;
    char *output_file;
    char *surface_type;
    int size;
    int version;
    unsigned int disable_egl_sample_buffers;
    char *shape_name;
    int x_position;
    int y_position;
    int width;
    int height;
    char *fill_type;
    char *fill_color;
    char *stroke_color;
    double red;
    double green;
    double blue;
    double alpha;
    int animation;
    char *stock_image_path;
    int stroke_width;
    int cap_style;
    int join_style;
    int dash_style;
    unsigned int enable_output_images;
    double tolerance;
    int canvas_width;
    int canvas_height;
    int list_tests;
    int list_gles_versions;
    int gles_version;
    char *drawing_lib;
    int list_drawing_libs;
    int depth_size;
    int luminance_size;
    int min_swap_interval;
    int max_swap_interval;
    int match_native_pixmap;
    int deferred_canvas;

    const char **tests;
} caskbench_options_t;

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
    printf("%s version %s+%.7s\n",
           PACKAGE_NAME,
           PACKAGE_VERSION,
           git_sha);
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

static void
print_tests_available()
{
    char *token = NULL;
    char test_name[MAX_BUFFER];
    for (int c = 0; c < num_perf_tests; c = c+2) {
        memset (test_name, 0, sizeof(test_name));
        strncpy (test_name, perf_tests[c].name, strlen(perf_tests[c].name));
        token = strtok (test_name, "-");
        if (token) {
            token = strtok (NULL, "-");
            printf("%s\n", token);
        }
    }
}

static void
print_gles_versions_available ()
{
#if HAVE_GLES2_H
    printf("2\n");
#endif
#if HAVE_GLES3_H
    printf("3\n");
#endif
}

static void
print_drawing_libs_available()
{
#if USE_CAIRO
    printf("cairo\n");
#endif
#if USE_SKIA
    printf("skia\n");
#endif
}

static void
print_json_result(FILE *fp, const caskbench_result_t *result)
{
    fprintf(fp, "   {\n");
    fprintf(fp, "       \"test case\": \"%s\",\n", result->test_case_name);
    fprintf(fp, "       \"size\": \"%d\",\n", result->size);
    fprintf(fp, "       \"status\": \"%s\",\n", _status_to_string(result->status));
    fprintf(fp, "       \"iterations\": %d,\n", result->iterations);
    fprintf(fp, "       \"minimum run time (s)\": %f,\n", result->min_run_time);
    fprintf(fp, "       \"average run time (s)\": %f,\n", result->avg_run_time);
    fprintf(fp, "       \"maximum run time (s)\": %f,\n", result->max_run_time);
    fprintf(fp, "       \"median run time (s)\": %f,\n", result->median_run_time);
    fprintf(fp, "       \"standard deviation of run time (s)\": %f,\n", result->standard_deviation);
    fprintf(fp, "       \"avg frames per second (fps)\": %-4.0f\n", result->avg_frames_per_second);
    fprintf(fp, "   }");
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
        {"json", 'j', POPT_ARG_NONE, &opt->json_output, 0,
         "Write JSON style output to stdout",
         NULL},
        {"list-surfaces", 'l', POPT_ARG_NONE, &opt->list_surfaces, 0,
         "List the available graphics surface types",
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
         "Display the program version and exit",
         NULL},
        {"disable-egl-sample-buffers", '\0', POPT_ARG_NONE, &opt->disable_egl_sample_buffers, 0,
         "Turn off sample buffers in EGL attributes",
         NULL},
        {"shape", 'S', POPT_ARG_STRING, &opt->shape_name, 0,
         "Controls which type of shape to draw for the test object (e.g. 'circle', 'star', etc.)",
         NULL},
        {"x-position", 'X', POPT_ARG_INT, &opt->x_position, 0,
         "The X location to draw the test object",
         NULL},
        {"y-position", 'Y', POPT_ARG_INT, &opt->y_position, 0,
         "The Y location to draw the test object",
         NULL},
        {"object-width", '\0', POPT_ARG_INT, &opt->width, 0,
         "Width of the test object",
         NULL},
        {"object-height", '\0', POPT_ARG_INT, &opt->height, 0,
         "Height of the test object",
         NULL},
        {"fill-type", 'f', POPT_ARG_STRING, &opt->fill_type, 0,
         "Use specified fill style ('solid', 'linear-gradient', 'radial-gradient', 'image-pattern')",
         NULL},
        {"fill-color", '\0', POPT_ARG_STRING, &opt->fill_color, 0,
         "RGBA color value for fill (e.g. ABCDEFFF)",
         NULL},
        {"stroke-color", '\0', POPT_ARG_STRING, &opt->stroke_color, 0,
         "RGBA color value for stroke (e.g. ABCDEFFF)",
         NULL},
        {"animation", 'g', POPT_ARG_INT, &opt->animation, 0,
         "Controls the kinematics of the objects drawn",
         NULL},
        {"image-path", 'I', POPT_ARG_STRING, &opt->stock_image_path, 0,
         "Path to a stock image for use in clipping, patterns, etc.",
         NULL},
        {"stroke-width", '\0', POPT_ARG_INT, &opt->stroke_width, 0,
         "Line width for stroked test objects",
         NULL},
#if 0
        {"cap-style", 'C', POPT_ARG_INT, &opt->cap_style, 0,
         "Line ending style for stroked test objects",
         NULL},
        {"join-style", 'J', POPT_ARG_INT, &opt->join_style, 0,
         "Corner type for multi-segmented, stroked test objects",
         NULL},
        {"dash-style", 'D', POPT_ARG_INT, &opt->dash_style, 0,
         "Dash style for stroked test objects",
         NULL},
#endif
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
        {"list-tests", '\0', POPT_ARG_NONE, &opt->list_tests, 0,
         "List the tests available",
         NULL},
#if USE_EGL
        {"list-gles-versions", '\0', POPT_ARG_NONE, &opt->list_gles_versions, 0,
         "List the GLES versions available",
         NULL},
        {"gles-version", '\0', POPT_ARG_INT, &opt->gles_version, 0,
         "GLES version to use",
         NULL},
        {"depth-size", '\0', POPT_ARG_INT, &opt->depth_size, 0,
         "EGL depth size to use",
         NULL},
        {"luminance-size", '\0', POPT_ARG_INT, &opt->luminance_size, 0,
         "EGL luminance size to use",
         NULL},
        {"min-swap-interval", '\0', POPT_ARG_INT, &opt->min_swap_interval, 0,
         "EGL minimum swap interval to use",
         NULL},
        {"max-swap-interval", '\0', POPT_ARG_INT, &opt->max_swap_interval, 0,
         "EGL maximum swap interval to use",
         NULL},
        {"match-native-pixmap", '\0', POPT_ARG_INT, &opt->match_native_pixmap, 0,
         "EGL pixmap handle to use",
         NULL},
        {"deferred-rendering", '\0', POPT_ARG_NONE, &opt->deferred_canvas, 0,
         "Creates Deferred Canvas for skia rendering for EGL backend. Deferred rendering records graphics commands and stores the intermediate result. It can be played back by the application appropriately.",
         NULL},
#endif
        {"list-drawing-libs", '\0', POPT_ARG_NONE, &opt->list_drawing_libs, 0,
         "List the drawing libraries available",
         NULL},
        {"drawing-lib", 'D', POPT_ARG_STRING, &opt->drawing_lib, 0,
         "Drawing libraries to be used for drawing (e.g. 'cairo', 'cairo,skia')",
         NULL},
        POPT_AUTOHELP
        {NULL}
    };

    // Initialize options
    memset (opt, 0, sizeof(caskbench_options_t));
    opt->iterations = 100;
    opt->size = 100;

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
    opt->tests = poptGetArgs(pc);

}

double
get_tick (void)
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return (double)now.tv_sec + (double)now.tv_usec / 1000000.0;
}

uint32_t
convertToColorValue (const char* hexString)
{
    char *end_ptr;
    uint32_t color_val = 0;
    int r, g, b;
    if ((hexString == NULL) || (strlen(hexString) != 8))
    {
        color_val = -1;
        return color_val;
    }
    color_val = strtoul(hexString, &end_ptr, 16);
    return color_val;
}

void
shape_defaults_init(shapes *shape_defaults, caskbench_options_t *opt)
{
    shape_defaults->x = opt->x_position;
    shape_defaults->y = opt->y_position;
    shape_defaults->dx1 = 0;
    shape_defaults->dy1 = 0;
    shape_defaults->dx2 = 0;
    shape_defaults->dy2 = 0;
    shape_defaults->width = opt->width;
    shape_defaults->height = opt->height;
    shape_defaults->shape_type = convertToShapeType(opt->shape_name);
    shape_defaults->fill_type = convertToFillType(opt->fill_type);
    shape_defaults->fill_color = convertToColorValue(opt->fill_color);
    shape_defaults->stroke_color = convertToColorValue(opt->stroke_color);
    shape_defaults->stroke_width = opt->stroke_width;
    shape_defaults->animation = opt->animation;
    shape_defaults->cap_style = opt->cap_style;
    shape_defaults->join_style = opt->join_style;
    shape_defaults->dash_style = opt->dash_style;
    shape_defaults->radius = 0.0;
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
        if (strcmp (perf_tests[j].name, test_name) == 0)
            return j;
    }

    return -1;
}

void
process_drawing_libs (const char *libs, int &num_tests, int *test_ids)
{
    int test_index = 0, j = 0;
    int index = 0, i = -1;
    char filter[MAX_BUFFER];
    bool skia_cairo = false;
    memset(filter, 0, sizeof(filter));

#ifndef USE_CAIRO
    if (strstr(libs, "cairo"))
        errx (0, "cairo is unavailable in the build\n");
#endif
#ifndef USE_SKIA
    if (strstr(libs, "skia"))
        errx (0, "skia is unavailable in this build\n");
#endif
    /* Caskbench runs as usual */
    if (strcmp(libs, "cairo,skia") == 0)
        return;

    if (strncmp(libs,"cairo", strlen(libs)) == 0)
        strncpy(filter, "cairo", 5);
    else if (strncmp(libs, "skia", strlen(libs)) == 0)
        strncpy(filter, "skia", 4);
    else if (strncmp(libs, "skia,cairo", strlen(libs)) == 0)
        skia_cairo = true;
    else
        errx (0, "Invalid library is specified\n");

    /* Handle the case if user has already provided list of tests to run */
    index = num_tests ? num_tests:num_perf_tests;
    for (j = 0; j < index; j++) {
        i = (test_ids[j]!=-1) ? test_ids[j]:j;
        if (strstr (perf_tests[i].name, filter) && !skia_cairo) {
            test_ids[test_index++] = i;
        }
        if (skia_cairo) {
            /* Add skia test followed by cairo */
            test_ids[test_index++] = i+1;
            test_ids[test_index++] = i;
            j++;
        }
    }
    num_tests = test_index;
}

void
populate_user_tests (const char** tests, int& num_tests, int* test_ids)
{
    int i = 0, id = -1, test_index = 0;
    char skia_test[MAX_BUFFER];
    char cairo_test[MAX_BUFFER];
    strncpy(cairo_test, "cairo-", 6);
    strncpy(skia_test, "skia-", 5);

    if (tests == NULL)
        return;
    while (tests[i] != NULL) {
        if (strlen(tests[i])+6 >= MAX_BUFFER)
            errx (0, "Invalid test case: test name is too long\n");

#ifdef USE_CAIRO
        cairo_test[6] = '\0';
        strncat(cairo_test, tests[i], strlen(tests[i]));

        /* Add cairo test index to the test list */
        id = convertToTestId (cairo_test);
        if (id < 0)
            errx (0, "Unrecognized cairo test case '%s'\n", cairo_test);
        test_ids[test_index++] = id;
        num_tests = num_tests + 1;
#endif
#ifdef USE_SKIA
        skia_test[5] = '\0';
        strncat(skia_test, tests[i], strlen(tests[i]));

        /* Add skia test index to the test list */
        id = convertToTestId (skia_test);
        if (id < 0)
            errx (0, "Unrecognized skia test case '%s'\n", skia_test);
        test_ids[test_index++] = id;
        num_tests = num_tests + 1;
#endif
        i++;
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
    double skia_avg_time = 0.0;
    FILE *fp = NULL;
    char fname[256], *end_ptr;
    device_config_t config;
    int foo;

    setenv("CAIRO_GL_COMPOSITOR", "msaa", 1);
    process_options(&opt, argc, argv);
    double run_time_values[opt.iterations];

    memset (&config, 0, sizeof(device_config_t));
    config.egl_min_swap_interval = -1;
    config.egl_max_swap_interval = -1;

    if (opt.version) {
        print_version();
        exit(0);
    }
    if (opt.list_surfaces) {
        print_surfaces_available();
        exit(0);
    }
    if (opt.list_tests) {
        print_tests_available();
        exit(0);
    }
    if(opt.list_gles_versions) {
        print_gles_versions_available();
        exit(0);
    }

#if HAVE_GLES3_H
    config.egl_gles_version = 3;
#elif HAVE_GLES2_H
    config.egl_gles_version = 2;
#endif

    if(opt.gles_version) {
        if (opt.surface_type == NULL || (strncmp(opt.surface_type,"image",5)==0))
            errx(0, "gles-version is not usable with image backend");
        if (opt.gles_version < 2 || opt.gles_version > 3)
            errx(0, "gles-version provided is invalid");
#ifndef HAVE_GLES2_H
        if (opt.gles_version == 2)
            errx(0, "gles-version 2 is not supported in the hardware");
#endif
#ifndef HAVE_GLES3_H
        if (opt.gles_version == 3)
            errx(0, "gles-version 3 is not supported in the hardware");
#endif
        config.egl_gles_version = opt.gles_version;
    }

    if (opt.list_drawing_libs) {
        print_drawing_libs_available();
        exit(0);
    }

    if (opt.output_file) {
        // start writing json to output file
        fp = fopen(opt.output_file, "w");
        fprintf(fp, "[\n");
    }
    if (opt.json_output) {
        fprintf(stdout, "[\n");
    }
    if (!opt.disable_egl_sample_buffers) {
        config.egl_samples = 4;
        config.egl_sample_buffers = 1;
    }

    if (opt.depth_size)
        config.egl_depth_size = opt.depth_size;
    if (opt.luminance_size)
        config.egl_luminance_size = opt.luminance_size;
    if (opt.min_swap_interval)
        config.egl_min_swap_interval = opt.min_swap_interval;
    if (opt.max_swap_interval)
        config.egl_max_swap_interval = opt.max_swap_interval;
    if (opt.match_native_pixmap)
        config.egl_match_native_pixmap = opt.match_native_pixmap;

    memset (user_test_ids, -1, sizeof(user_test_ids));
    populate_user_tests (opt.tests, num_user_tests, user_test_ids);
    if(opt.drawing_lib)
        process_drawing_libs (opt.drawing_lib, num_user_tests, user_test_ids);

    num_tests = num_user_tests ? num_user_tests : num_perf_tests;
    for(int s = 0; s < num_tests; s++) {
        c = num_user_tests ? user_test_ids[s] : s;
        caskbench_context_t context;
        caskbench_result_t result;

        /* Disable stroke: Displays no output to file */
        if (!strncmp(perf_tests[c].name, "cairo-stroke", 12) ||
            !strncmp(perf_tests[c].name, "skia-stroke", 11))
            continue;

        /* Reinitialize random seed to a known state */
        srnd();

        context_init(&context, opt.size);
        if(opt.canvas_width)
            context.canvas_width = opt.canvas_width;
        if(opt.canvas_height)
            context.canvas_height = opt.canvas_height;
        if (opt.stock_image_path)
            context.stock_image_path = opt.stock_image_path;
        if (opt.surface_type && opt.deferred_canvas)
            context.is_egl_deferred = !strncmp(opt.surface_type, "egl", 3);
        else if (opt.deferred_canvas)
            errx (0, "Deferred rendering is not available with the current configuration");

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
                // Clearing the canvas between iterations
                perf_tests[c].context_clear(&context);

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
        if (! opt.json_output) {
            printf("%-20s %-4d   %s  %d  %4.0f",
                   result.test_case_name,
                   result.size,
                   _status_to_string(result.status),
                   result.iterations,
                   result.avg_frames_per_second);

            if (result.test_case_name[0] == 'c')
                cairo_avg_time = result.avg_run_time;
            if (result.test_case_name[0] == 's')
                skia_avg_time = result.avg_run_time;

            if (s%2 == 1) {
                if (opt.drawing_lib != NULL) {
                    if ((strcmp(opt.drawing_lib, "cairo,skia") == 0)) {
                        double perf_improvement = (cairo_avg_time - result.avg_run_time)/cairo_avg_time;
                        printf("  %4.2f%%", perf_improvement * 100.0);
                        cairo_avg_time = 0.0;
                    }
                    else if ((strcmp(opt.drawing_lib, "skia,cairo") == 0)) {
                        double perf_improvement = (skia_avg_time - result.avg_run_time)/skia_avg_time;
                        printf("  %4.2f%%", perf_improvement * 100.0);
                        skia_avg_time = 0.0;
                    }
                } else {
                    double perf_improvement = (cairo_avg_time - result.avg_run_time)/cairo_avg_time;
                    printf("  %4.2f%%", perf_improvement * 100.0);
                    cairo_avg_time = 0.0;
                }
            }
        } else {
            print_json_result(stdout, &result);

            if (s < num_tests-1)
                fprintf(stdout, ",");
            fprintf(stdout, "\n");
        }
        printf("\n");

        if (fp != NULL) {
            print_json_result(fp, &result);

            if (s < num_tests-1)
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
    if (opt.json_output) {
        // End json
        fprintf(stdout, "]\n");
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
