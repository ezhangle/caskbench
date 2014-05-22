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
    int status;
} caskbench_result_t;

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
}

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

