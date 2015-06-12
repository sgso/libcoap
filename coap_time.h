/* coap_time.h -- Clock Handling
 *
 * Copyright (C) 2010--2013 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use. 
 */

/** 
 * @file coap_time.h
 * @brief Clock Handling
 */

#ifndef _COAP_TIME_H_
#define _COAP_TIME_H_

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


#include "config.h"

/**
 * @defgroup clock Clock Handling
 * Default implementation of internal clock. You should redefine this if
 * you do not have time() and gettimeofday().
 * @{
 */

#include <vtimer.h>

#define COAP_TICKS_PER_SECOND 1024

typedef unsigned int coap_tick_t;

typedef int coap_tick_diff_t;

/* We don't need an offset, we take the uptime */
#define coap_clock_init()

static inline void riot_ticks_impl(coap_tick_t *t)
{
    struct timeval tp;
    vtimer_gettimeofday(&tp);
    *t = (tp.tv_sec) * COAP_TICKS_PER_SECOND
       + (tp.tv_usec * COAP_TICKS_PER_SECOND / 1000000);
}

#define coap_ticks riot_ticks_impl
extern time_t clock_offset;

#ifndef coap_clock_init
static inline void
coap_clock_init_impl(void) {
#ifdef HAVE_TIME_H
  clock_offset = time(NULL);
#else
#  ifdef __GNUC__
    /* Issue a warning when using gcc. Other prepropressors do 
     *  not seem to have a similar feature. */ 
#   warning "cannot initialize clock"
#  endif
  clock_offset = 0;
#endif
}
#define coap_clock_init coap_clock_init_impl
#endif /* coap_clock_init */

#ifndef coap_ticks
static inline void
coap_ticks_impl(coap_tick_t *t) {
#ifdef HAVE_SYS_TIME_H
  struct timeval tv;
  gettimeofday(&tv, NULL);
  *t = (tv.tv_sec - clock_offset) * COAP_TICKS_PER_SECOND 
    + (tv.tv_usec * COAP_TICKS_PER_SECOND / 1000000);
#else
#error "clock not implemented"
#endif
}
#define coap_ticks coap_ticks_impl
#endif /* coap_ticks */

/**
 * Returns @c 1 if and only if @p a is less than @p b where less is
 * defined on a signed data type.
 */
static inline
int coap_time_lt(coap_tick_t a, coap_tick_t b) {
  return ((coap_tick_diff_t)(a - b)) < 0;
}

/**
 * Returns @c 1 if and only if @p a is less than or equal @p b where
 * less is defined on a signed data type.
 */
static inline
int coap_time_le(coap_tick_t a, coap_tick_t b) {
  return a == b || coap_time_lt(a,b);
}

/** @} */

#ifdef __cplusplus
}
#endif


#endif /* _COAP_TIME_H_ */
