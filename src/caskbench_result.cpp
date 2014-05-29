/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <assert.h>

#include "caskbench_result.h"

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
    result->status = 0;
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
