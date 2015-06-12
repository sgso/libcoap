#include "mem.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>

void coap_memory_init(void)
{
}

void *coap_malloc_type(coap_memory_tag_t type, size_t size)
{
    (void)type;
    return malloc(size);
}

void coap_free_type(coap_memory_tag_t type, void *p)
{
    (void)type;
    free(p);
}
