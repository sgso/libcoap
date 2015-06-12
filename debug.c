#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "net.h"

#ifndef NDEBUG

unsigned int print_readable(const unsigned char *data, unsigned int len,
                            unsigned char *result, unsigned int buflen,
                            int encode_always)
{
    const unsigned char hex[] = "0123456789ABCDEF";
    unsigned int cnt = 0;
    assert(data || len == 0);

    if (buflen == 0 || len == 0) {
        return 0;
    }

    while (len) {
        if (!encode_always && isprint(*data)) {
            if (cnt == buflen) {
                break;
            }

            *result++ = *data;
            ++cnt;
        }
        else {
            if (cnt + 4 < buflen) {
                *result++ = '\\';
                *result++ = 'x';
                *result++ = hex[(*data & 0xf0) >> 4];
                *result++ = hex[*data & 0x0f];
                cnt += 4;
            }
            else {
                break;
            }
        }

        ++data;
        --len;
    }

    *result = '\0';
    return cnt;
}

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

size_t coap_print_addr(const struct coap_address_t *addr,
                       unsigned char *buf, size_t len)
{
    (void)addr;
    (void)buf;
    (void)len;
    /* TODO: output address */
    return 0;
}


void coap_show_pdu(const coap_pdu_t *pdu)
{
    unsigned char buf[COAP_MAX_PDU_SIZE]; /* need some space for output creation */
    int encode = 0, have_options = 0;
    coap_opt_iterator_t opt_iter;
    coap_opt_t *option;

    fprintf(COAP_DEBUG_FD, "v:%d t:%d tkl:%d c:%d id:%u",
            pdu->hdr->version, pdu->hdr->type,
            pdu->hdr->token_length,
            pdu->hdr->code, ntohs(pdu->hdr->id));

    /* show options, if any */
    coap_option_iterator_init((coap_pdu_t *)pdu, &opt_iter, COAP_OPT_ALL);

    while ((option = coap_option_next(&opt_iter))) {
        if (!have_options) {
            have_options = 1;
            fprintf(COAP_DEBUG_FD, " o: [");
        }
        else {
            fprintf(COAP_DEBUG_FD, ",");
        }

        if (opt_iter.type == COAP_OPTION_URI_PATH ||
            opt_iter.type == COAP_OPTION_PROXY_URI ||
            opt_iter.type == COAP_OPTION_URI_HOST ||
            opt_iter.type == COAP_OPTION_LOCATION_PATH ||
            opt_iter.type == COAP_OPTION_LOCATION_QUERY ||
            opt_iter.type == COAP_OPTION_URI_PATH ||
            opt_iter.type == COAP_OPTION_URI_QUERY) {
            encode = 0;
        }
        else {
            encode = 1;
        }

        if (print_readable(COAP_OPT_VALUE(option),
                           COAP_OPT_LENGTH(option),
                           buf, sizeof(buf), encode)) {
            fprintf(COAP_DEBUG_FD, " %d:'%s'", opt_iter.type, buf);
        }
    }

    if (have_options) {
        fprintf(COAP_DEBUG_FD, " ]");
    }

    if (pdu->data) {
        assert(pdu->data < (unsigned char *)pdu->hdr + pdu->length);
        print_readable(pdu->data,
                       (unsigned char *)pdu->hdr + pdu->length - pdu->data,
                       buf, sizeof(buf), 0);
        fprintf(COAP_DEBUG_FD, " d:%s", buf);
    }

    fprintf(COAP_DEBUG_FD, "\n");
    fflush(COAP_DEBUG_FD);
}

#endif /* NDEBUG */
