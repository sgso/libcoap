#ifndef COAP_DEBUG_H_
#define COAP_DEBUG_H_

#include "log.h"

#ifndef COAP_DEBUG_FD
#define COAP_DEBUG_FD stdout
#endif

#ifndef COAP_ERR_FD
#define COAP_ERR_FD stderr
#endif

#define LOG_EMERG       LOG_ERROR
#define LOG_ALERT       LOG_ERROR
#define LOG_CRIT        LOG_ERROR
#define LOG_NOTICE      LOG_INFO

/** Returns the (static) log level. */
#define coap_get_log_level()  LOG_LEVEL

#define coap_log(level, ...) LOG(level, __VA_ARGS__)

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

#endif /* COAP_DEBUG_H_ */
