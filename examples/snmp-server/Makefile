CONTIKI_PROJECT = snmp-server 
all: $(CONTIKI_PROJECT)

MODULES += os/net/app-layer/snmp

MODULES_REL += ./resources

MODULES += os/services/orchestra

MAKE_MAC = MAKE_MAC_TSCH

CONTIKI = ../..

include $(CONTIKI)/Makefile.include
