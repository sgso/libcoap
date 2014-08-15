#include "config.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "debug.h"
#include "mem.h"
#include "coap_io.h"

#include "sixlowpan/ip.h"
#include "destiny/socket.h"

static inline struct coap_endpoint_t *
coap_malloc_posix_endpoint(void) {
  return (struct coap_endpoint_t *)coap_malloc(sizeof(struct coap_endpoint_t));
}

static inline void
coap_free_posix_endpoint(struct coap_endpoint_t *ep) {
  coap_free(ep);
}

coap_endpoint_t *coap_new_endpoint(const coap_address_t *addr, int flags) {
  int sockfd = destiny_socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

  struct coap_endpoint_t *ep;

  if (sockfd < 0) {
    coap_log(LOG_WARNING, "coap_new_endpoint: socket");
    return NULL;
  }

  if (destiny_socket_bind(sockfd, (coap_address_t*)addr, 16) < 0) {
    coap_log(LOG_WARNING, "coap_new_endpoint: bind");
    destiny_socket_close(sockfd);
    return NULL;
  }

  ep = coap_malloc_posix_endpoint();
  if (!ep) {
    coap_log(LOG_WARNING, "coap_new_endpoint: malloc");
    close(sockfd);
    return NULL;
  }

  memset(ep, 0, sizeof(struct coap_endpoint_t));
  ep->handle = sockfd;
  ep->flags = flags;
  memcpy(&ep->addr, addr, sizeof(coap_address_t));
  
#ifndef NDEBUG
  if (LOG_DEBUG <= coap_get_log_level()) {
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 40
#endif
    unsigned char addr_str[INET6_ADDRSTRLEN+8];

    if (coap_print_addr(addr, addr_str, INET6_ADDRSTRLEN+8)) {
      debug("created %sendpoint %s\n", 
	    ep->flags & COAP_ENDPOINT_DTLS ? "DTLS " : "",
	    addr_str);
    }
    destiny_socket_print_sockets();
  }
#endif /* NDEBUG */

  return (coap_endpoint_t *)ep;
}

void
coap_free_endpoint(coap_endpoint_t *ep) {
  if(ep) {
    if (ep->handle >= 0)
      close(ep->handle);
    coap_free_posix_endpoint((struct coap_endpoint_t *)ep);
  }
}

#ifdef __GNUC__
#define UNUSED_PARAM __attribute__ ((unused))
#else /* not a GCC */
#define UNUSED_PARAM
#endif /* GCC */

ssize_t
coap_network_send(struct coap_context_t *context UNUSED_PARAM,
		  const coap_endpoint_t *local_interface,
		  const coap_address_t *dst,
		  unsigned char *data,
		  size_t datalen) 
{
    struct coap_endpoint_t *ep = 
        (struct coap_endpoint_t *)local_interface;

    assert(local_interface);
  
    return destiny_socket_sendto(ep->handle,
                                 (void*)data,
                                 datalen,
                                 0,
                                 dst,
                                 16);
}

static coap_packet_t *coap_malloc_packet(void) {
    coap_packet_t *packet;
    const size_t need = sizeof(coap_packet_t) + COAP_MAX_PDU_SIZE;

    packet = (coap_packet_t *)coap_malloc(need);
    if (packet) {
        memset(packet, 0, need);
    }
    return packet;
}

void coap_free_packet(coap_packet_t *packet) {
    coap_free(packet);
}

static inline size_t coap_get_max_packetlength(const coap_packet_t *packet UNUSED_PARAM) {
  return COAP_MAX_PDU_SIZE;
}

#ifdef WITH_DESTINY_TIMEOUT
ssize_t coap_network_read(coap_endpoint_t *ep, coap_packet_t **packet, timex_t* timeout) {
#else
ssize_t coap_network_read(coap_endpoint_t *ep, coap_packet_t **packet) {
#endif /* WITH_DESTINY_TIMEOUT */

    ssize_t len = -1;
    socklen_t from_len = 0;

    assert(ep);
    assert(packet);

    *packet = coap_malloc_packet();
  
    if (!*packet) {
        warn("coap_network_read: insufficient memory, drop packet\n");
        return -1;
    }

    coap_address_init(&(*packet)->dst); /* the local interface address */
    coap_address_init(&(*packet)->src); /* the remote peer */

#ifdef WITH_DESTINY_TIMEOUT
    if (timeout) {
        len = destiny_socket_recvfrom_timeout(ep->handle,
                                              &(*packet)->payload,
                                              COAP_MAX_PDU_SIZE,
                                              0,
                                              &(*packet)->src,
                                              &from_len,
                                              timeout);
    } else {
#endif /* WITH_DESTINY_TIMEOUT */
        len = destiny_socket_recvfrom(ep->handle,
                                      &(*packet)->payload,
                                      COAP_MAX_PDU_SIZE,
                                      0,
                                      &(*packet)->src,
                                      &from_len);
#ifdef WITH_DESTINY_TIMEOUT        
    }
#endif /* WITH_DESTINY_TIMEOUT */

    if (len < 0) {
        coap_free_packet(*packet);
        *packet = NULL;
    } else {
        assert(from_len == sizeof(coap_address_t));
        (*packet)->length = len;
        (*packet)->ifindex = 0;
        memcpy(&(*packet)->dst, &ep->addr, sizeof(coap_address_t));
    }

    return len;
}
