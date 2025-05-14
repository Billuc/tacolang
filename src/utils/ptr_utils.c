#include "ptr_utils.h"
#include <string.h>

void *ptr_copy(void *ptr, size_t size)
{
    if (ptr == NULL)
    {
        return NULL;
    }

    // Allocate new memory and copy the contents
    void *new_ptr = malloc(size);
    if (new_ptr == NULL)
    {
        return NULL;
    }

    memcpy(new_ptr, ptr, size);
    return new_ptr;
}