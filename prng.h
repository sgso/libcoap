/**
 * @file prng.h
 * @brief Pseudo Random Numbers
 */

#ifndef COAP_PRNG_H_
#define COAP_PRNG_H_

#include "config.h"

/**
 * @defgroup prng Pseudo Random Numbers
 * @{
 */

#include <stdlib.h>

/**
 * Fills \p buf with \p len random bytes. This is the default
 * implementation for prng().  You might want to change prng() to use
 * a better PRNG on your specific platform.
 */
static inline int coap_prng_impl(unsigned char *buf, size_t len)
{
    while (len--) {
        *buf++ = rand() & 0xFF;
    }

    return 1;
}

#ifndef prng
/**
 * Fills \p Buf with \p Length bytes of random data.
 *
 * @hideinitializer
 */
#define prng(Buf,Length) coap_prng_impl((Buf), (Length))
#endif

#ifndef prng_init
/**
 * Called to set the PRNG seed. You may want to re-define this to
 * allow for a better PRNG.
 *
 * @hideinitializer
 */
#define prng_init(Value) srand((unsigned long)(Value))
#endif

#endif /* COAP_PRNG_H_ */
