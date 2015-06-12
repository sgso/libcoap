/* debug.h -- debug utilities
 *
 * Copyright (C) 2010,2011,2014 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#ifndef _COAP_DEBUG_H_
#define _COAP_DEBUG_H_

#include "config.h"

#ifndef COAP_DEBUG_FD
#define COAP_DEBUG_FD stdout
#endif

#ifndef COAP_ERR_FD
#define COAP_ERR_FD stderr
#endif

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
typedef short coap_log_t;
#else
#include "log.h"
#define LOG_EMERG       LOG_ERROR
#define LOG_ALERT       LOG_ERROR
#define LOG_CRIT        LOG_ERROR
#define LOG_NOTICE      LOG_INFO
#endif

/** Returns the (static) log level. */
#define coap_get_log_level()  LOG_LEVEL

#ifndef coap_log
#define coap_log(level, ...) LOG(level, __VA_ARGS__)
#endif

#ifndef NDEBUG

/* A set of convenience macros for common log levels. */
#define warn(...) LOG_WARNING(__VA_ARGS__)
#define debug(...) LOG_DEBUG(__VA_ARGS__)

#include "pdu.h"
void coap_show_pdu(const coap_pdu_t *);

struct coap_address_t;
size_t coap_print_addr(const struct coap_address_t *, unsigned char *, size_t);

#else

#define debug(...)
#define warn(...)

#define coap_show_pdu(x)
#define coap_print_addr(...)

#endif

#endif /* _COAP_DEBUG_H_ */
