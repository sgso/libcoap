#include "config.h"

#include <stdio.h>

#include "debug.h"
#include "mem.h"
#include "str.h"

str *coap_new_string(size_t size)
{
    str *s = coap_malloc(sizeof(str) + size + 1);

    if (!s) {
#ifndef NDEBUG
        coap_log(LOG_CRIT, "coap_new_string: malloc\n");
#endif
        return NULL;
    }

    memset(s, 0, sizeof(str));
    s->s = ((unsigned char *)s) + sizeof(str);
    return s;
}

void coap_delete_string(str *s)
{
    coap_free(s);
}

