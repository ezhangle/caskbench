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
