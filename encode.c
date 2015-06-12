#include "config.h"
#include "encode.h"

/* Carsten suggested this when fls() is not available: */
int coap_fls(unsigned int i)
{
    int n;

    for (n = 0; i; n++) {
        i >>= 1;
    }

    return n;
}

unsigned int coap_decode_var_bytes(unsigned char *buf, unsigned int len)
{
    unsigned int i, n = 0;

    for (i = 0; i < len; ++i) {
        n = (n << 8) + buf[i];
    }

    return n;
}

unsigned int coap_encode_var_bytes(unsigned char *buf, unsigned int val)
{
    unsigned int n, i;

    for (n = 0, i = val; i && n < sizeof(val); ++n) {
        i >>= 8;
    }

    i = n;

    while (i--) {
        buf[i] = val & 0xff;
        val >>= 8;
    }

    return n;
}

