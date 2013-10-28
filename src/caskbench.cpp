#include <sys/time.h>

#include "framework.h"

typedef struct _caskbench_perf_case {
  PERF_RUN_DECL (*run);
} caskbench_perf_case_t;

const caskbench_perf_case_t perf_cases[];

double
cb_get_tick (void)
{
  struct timeval now;
  gettimeofday (&now, NULL);
  return (double)now.tv_sec + (double)now.tv_usec / 1000000.0;
}

void
cb_generate_color (double *color)
{
  int i;

  for (i = 0; i< 3; i++)
    color[i] = (int)(drand48 () * 3)/3.0;
}

#define NUM_CASES (10)

// TODO: parse_options

int
main (int argc, char *argv[])
{
  int c, i;
  int num_iterations = 10;
  double start_time, stop_time, run_sec;

  parse_options (&perf, argc, argv);

  // TODO: Open output file, start json
  for (c=0; c<NUM_CASES; c++) {
    test_case = NULL; // TODO
    // If dry run, just list the test

    // TODO: Perhaps run once just to warm caches and calibrate
    start_time = cb_get_tick();
    for (i=0; i<num_iterations; i++) {
    try:
      // TODO: Execute test_case
    except:
      // Mark as crashed
    }
    stop_time = cb_get_tick();
    run_sec = stop_time - start_time;
    // TODO: Print test summary to stdout
    // TODO: Write json
  }
  // TODO: End json, close output file
}
