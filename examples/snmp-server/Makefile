CONTIKI_PROJECT = snmp-server 
all: $(CONTIKI_PROJECT)

MODULES += os/services/orchestra
MODULES += os/services/network-energest

MAKE_MAC = MAKE_MAC_TSCH

MODULES += os/net/app-layer/snmp

MODULES_REL += ./resources

CONTIKI = ../..

include $(CONTIKI)/Makefile.include
