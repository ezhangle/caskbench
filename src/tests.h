#ifndef __TESTS_H__
#define __TESTS_H__

struct _caskbench_context;

typedef struct _caskbench_perf_test {
  const char *name;
  int (*setup)(struct _caskbench_context*);
  void (*teardown)(void);
  int (*test_case)(struct _caskbench_context*);
  void (*write_image)(const char *, struct _caskbench_context*);
} caskbench_perf_test_t;

extern caskbench_perf_test_t perf_tests[];
extern const int num_perf_tests;

void   write_image_file_cairo(const char *fname, struct _caskbench_context *context);
void   write_image_file_skia(const char *fname, struct _caskbench_context *context);

#endif // __TESTS_H__
