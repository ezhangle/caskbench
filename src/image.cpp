#include <config.h>

#include <err.h>
#include <stdlib.h>

#include "image.h"

void
cleanup_state_image (void *data)
{
    image_state_t *state = (image_state_t*)data;
    free(state);
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
