#include <config.h>

#include "tests.h"

// Cairo Tests
int  ca_setup_fill(struct _caskbench_context *ctx);
void ca_teardown_fill();
int  ca_test_fill(struct _caskbench_context *ctx);

int  ca_setup_image(struct _caskbench_context *ctx);
void ca_teardown_image();
int  ca_test_image(struct _caskbench_context *ctx);

int  ca_setup_mask(struct _caskbench_context *ctx);
void ca_teardown_mask();
int  ca_test_mask(struct _caskbench_context *ctx);

int  ca_setup_paint(struct _caskbench_context *ctx);
void ca_teardown_paint();
int  ca_test_paint(struct _caskbench_context *ctx);

int  ca_setup_stroke(struct _caskbench_context *ctx);
void ca_teardown_stroke();
int  ca_test_stroke(struct _caskbench_context *ctx);

int  ca_setup_fill(struct _caskbench_context *ctx);
void ca_teardown_fill();
int  ca_test_fill(struct _caskbench_context *ctx);

int  ca_setup_image(struct _caskbench_context *ctx);
void ca_teardown_image();
int  ca_test_image(struct _caskbench_context *ctx);

int  ca_setup_mask(struct _caskbench_context *ctx);
void ca_teardown_mask();
int  ca_test_mask(struct _caskbench_context *ctx);

int  ca_setup_paint(struct _caskbench_context *ctx);
void ca_teardown_paint();
int  ca_test_paint(struct _caskbench_context *ctx);

int  ca_setup_stroke(struct _caskbench_context *ctx);
void ca_teardown_stroke();
int  ca_test_stroke(struct _caskbench_context *ctx);

int  ca_setup_stroke(struct _caskbench_context *ctx);
void ca_teardown_stroke();
int  ca_test_stroke(struct _caskbench_context *ctx);

int  ca_setup_fill(struct _caskbench_context *ctx);
void ca_teardown_fill();
int  ca_test_fill(struct _caskbench_context *ctx);

int  ca_setup_image(struct _caskbench_context *ctx);
void ca_teardown_image();
int  ca_test_image(struct _caskbench_context *ctx);

int  ca_setup_mask(struct _caskbench_context *ctx);
void ca_teardown_mask();
int  ca_test_mask(struct _caskbench_context *ctx);

int  ca_setup_paint(struct _caskbench_context *ctx);
void ca_teardown_paint();
int  ca_test_paint(struct _caskbench_context *ctx);

int  ca_setup_rectangles(struct _caskbench_context *ctx);
void ca_teardown_rectangles();
int  ca_test_rectangles(struct _caskbench_context *ctx);

int  ca_setup_roundrect(struct _caskbench_context *ctx);
void ca_teardown_roundrect();
int  ca_test_roundrect(struct _caskbench_context *ctx);

int  ca_setup_bubbles(struct _caskbench_context *ctx);
void ca_teardown_bubbles();
int  ca_test_bubbles(struct _caskbench_context *ctx);

int  ca_setup_multishape(struct _caskbench_context *ctx);
void ca_teardown_multishape();
int  ca_test_multishape(struct _caskbench_context *ctx);


// Skia Tests
int  sk_setup_fill(struct _caskbench_context *ctx);
void sk_teardown_fill();
int  sk_test_fill(struct _caskbench_context *ctx);

int  sk_setup_image(struct _caskbench_context *ctx);
void sk_teardown_image();
int  sk_test_image(struct _caskbench_context *ctx);

int  sk_setup_mask(struct _caskbench_context *ctx);
void sk_teardown_mask();
int  sk_test_mask(struct _caskbench_context *ctx);

int  sk_setup_paint(struct _caskbench_context *ctx);
void sk_teardown_paint();
int  sk_test_paint(struct _caskbench_context *ctx);

int  sk_setup_stroke(struct _caskbench_context *ctx);
void sk_teardown_stroke();
int  sk_test_stroke(struct _caskbench_context *ctx);

int  sk_setup_rectangles(struct _caskbench_context *ctx);
void sk_teardown_rectangles();
int  sk_test_rectangles(struct _caskbench_context *ctx);

int  sk_setup_roundrect(struct _caskbench_context *ctx);
void sk_teardown_roundrect();
int  sk_test_roundrect(struct _caskbench_context *ctx);

int  sk_setup_bubbles(struct _caskbench_context *ctx);
void sk_teardown_bubbles();
int  sk_test_bubbles(struct _caskbench_context *ctx);

int  sk_setup_multishape(struct _caskbench_context *ctx);
void sk_teardown_multishape();
int  sk_test_multishape(struct _caskbench_context *ctx);

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

const int num_perf_tests = sizeof(perf_tests) / sizeof(perf_tests[0]);
