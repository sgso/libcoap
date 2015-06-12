MODULE:=$(shell basename $(CURDIR))
INCLUDES += -I$(RIOTBASE) -I$(RIOTBASE)/sys/include -I$(RIOTBASE)/core/include -I$(RIOTBASE)/drivers/include

include $(RIOTBASE)/Makefile.base
