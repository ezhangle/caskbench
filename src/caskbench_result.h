/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef __CASKBENCH_RESULT_H__    // -*- mode: c++ -*-
#define __CASKBENCH_RESULT_H__

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

void result_init(caskbench_result_t *result, const char* name);

#endif // __CASKBENCH_RESULT_H__

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
