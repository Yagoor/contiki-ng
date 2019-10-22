CONTIKI_PROJECT = comp-server 
all: $(CONTIKI_PROJECT)

TARGET=cc26x0-cc13x0

MAKE_MAC = MAKE_MAC_TSCH

MODULES += os/net/app-layer/comp
MODULES += os/services/orchestra

MODULES_REL += ./resources

CONTIKI = ../..

include $(CONTIKI)/Makefile.include
