/**
 * @file  address.h
 * @brief representation of network addresses
 */

#ifndef COAP_ADDRESS_H_
#define COAP_ADDRESS_H_

#include "config.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "net/ng_ipv6/addr.h"

typedef struct __attribute__((packed)) {
    ng_ipv6_addr_t addr;
    uint16_t port;
} coap_address_t;

/**
 * Resets the given coap_address_t object @p addr to its default
 * values.  In particular, the member size must be initialized to the
 * available size for storing addresses.
 *
 * @param addr The coap_address_t object to initialize.
 */
static inline void coap_address_init(coap_address_t *addr)
{
    assert(addr);
    memset(addr, 0, sizeof(coap_address_t));
}

/**
 * Compares given address objects @p a and @p b. This function returns
 * @c 1 if addresses are equal, @c 0 otherwise. The parameters @p a
 * and @p b must not be @c NULL;
 */
static inline int coap_address_equals(const coap_address_t *a, const coap_address_t *b)
{
    if (a->port != b->port) {
        return 0;
    }

    return ng_ipv6_addr_equal(&a->addr, &b->addr);
}

/**
 * Checks if given address object @p a denotes the wildcard
 * address. This function returns @c 1 if this is the case, @c 0
 * otherwise. The parameters @p a must not be @c NULL;
 */
static inline int coap_address_isany(const coap_address_t *a)
{
    return ng_ipv6_addr_is_unspecified(&a->addr);
}

/**
 * Checks if given address @p a denotes a multicast address.  This
 * function returns @c 1 if @p a is multicast, @c 0 otherwise.
 */
static inline int coap_is_mcast(const coap_address_t *a)
{
    (void)a;
    return 0;                   /* not implemented */
}

#endif /* COAP_ADDRESS_H_ */
