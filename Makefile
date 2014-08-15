MODULE:=$(shell basename $(CURDIR))
INCLUDES += -I$(RIOTBASE) -I$(RIOTBASE)/sys/include -I$(RIOTBASE)/core/include -I$(RIOTBASE)/drivers/include -I$(RIOTBASE)/sys/net/destiny/include -I$(RIOTBASE)/sys/net/sixlowpan/include/ -I$(RIOTBASE)/sys/net/ieee802154/include -I$(RIOTBASE)/sys/net/net_help
CFLAGS += -DWITH_POSIX -DWITH_RIOT

include $(RIOTBASE)/Makefile.base
