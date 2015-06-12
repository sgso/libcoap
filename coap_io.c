
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "coap_io.h"
#include "debug.h"
#include "mem.h"
#include "net/ng_netbase.h"
#include "net/ng_netreg.h"

coap_endpoint_t *coap_new_endpoint(const coap_address_t *addr, int flags)
{
    coap_endpoint_t *ep;
    ep = (struct coap_endpoint_t *)coap_malloc(sizeof(struct coap_endpoint_t));

    if (!ep) {
        coap_log(LOG_WARNING, "coap_new_endpoint: malloc");
        return NULL;
    }

    memset(ep, 0, sizeof(coap_endpoint_t));
    ep->flags = flags;
    memcpy(&ep->addr, addr, sizeof(coap_address_t));

    return ep;
}

void coap_free_endpoint(coap_endpoint_t *ep) {
    if (ep) {
        coap_free(ep);
    }
}

ssize_t coap_network_send(struct coap_context_t *context,
                          const coap_endpoint_t *local_interface,
                          const coap_address_t *dst,
                          coap_pdu_t *pdu)
{
    (void)context;
    ng_netreg_entry_t *sendto;

    ng_pktsnip_t *pdu_snip =
        ng_pktbuf_add(NULL, pdu->hdr, pdu->length,
                      NG_NETTYPE_UNDEF);

    ng_pktsnip_t *udp_snip =
        ng_netreg_hdr_build(NG_NETTYPE_UDP, pdu_snip,
                            (uint8_t *)&local_interface->addr.port, sizeof(uint16_t),
                            (uint8_t *)&dst->port, sizeof(uint16_t));

    ng_pktsnip_t *ipv6_snip =
        ng_netreg_hdr_build(NG_NETTYPE_IPV6, udp_snip,
                            (uint8_t *)&local_interface->addr.addr, sizeof(ng_ipv6_addr_t),
                            (uint8_t *)&dst->addr, sizeof(ng_ipv6_addr_t));

    /* and forward packet to the network layer */
    sendto = ng_netreg_lookup(NG_NETTYPE_UDP, NG_NETREG_DEMUX_CTX_ALL);

    /* throw away packet if no one is interested */
    if (sendto == NULL) {
        debug("coap_network_send(): cannot send packet because network layer not found\n");
        ng_pktbuf_release(ipv6_snip);
        return -1;
    }

    /* send packet to network layer */
    ng_pktbuf_hold(ipv6_snip, ng_netreg_num(NG_NETTYPE_UDP, NG_NETREG_DEMUX_CTX_ALL) - 1);

    while (sendto != NULL) {
        ng_netapi_send(sendto->pid, ipv6_snip);
        sendto = ng_netreg_getnext(sendto);
    }

    return pdu->length;
}

void coap_free_packet(coap_packet_t *packet)
{
    /* checks for NULL itself */
    ng_pktbuf_release(packet);
}

static inline size_t coap_get_max_packetlength(const coap_packet_t *packet)
{
    (void)packet;
    return COAP_MAX_PDU_SIZE;
}

/* Not needed on RIOT */
ssize_t coap_network_read(coap_endpoint_t *ep, coap_packet_t **packet)
{
    (void)ep;
    (void)packet;
    return 0;
}
