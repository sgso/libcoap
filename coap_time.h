/**
 * @file coap_time.h
 * @brief Clock Handling
 */

#ifndef COAP_TIME_H_
#define COAP_TIME_H_

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "vtimer.h"
    
/**
 * @defgroup clock Clock Handling
 * Default implementation of internal clock. You should redefine this if
 * you do not have time() and gettimeofday().
 * @{
 */

#define COAP_TICKS_PER_SECOND 1024

typedef unsigned int coap_tick_t;

typedef int coap_tick_diff_t;
extern time_t clock_offset;
    
static inline void coap_ticks(coap_tick_t *t)
{
    struct timeval tp;
    vtimer_gettimeofday(&tp);
    *t =  (tp.tv_sec) * COAP_TICKS_PER_SECOND
        + (tp.tv_usec * COAP_TICKS_PER_SECOND / 1000000);
}

static inline void coap_clock_init(void)
{
    clock_offset = time(NULL);
}

/**
 * Returns @c 1 if and only if @p a is less than @p b where less is
 * defined on a signed data type.
 */
static inline int coap_time_lt(coap_tick_t a, coap_tick_t b)
{
    return ((coap_tick_diff_t)(a - b)) < 0;
}

/**
 * Returns @c 1 if and only if @p a is less than or equal @p b where
 * less is defined on a signed data type.
 */
static inline int coap_time_le(coap_tick_t a, coap_tick_t b)
{
    return a == b || coap_time_lt(a, b);
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* COAP_TIME_H_ */
