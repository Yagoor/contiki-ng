CONTIKI_PROJECT = comp-server 
all: $(CONTIKI_PROJECT)

TARGET ?= native

ifeq ("$(TARGET)","native")
MAKE_MAC = MAKE_MAC_CSMA
else
MAKE_MAC = MAKE_MAC_TSCH
MODULES += os/services/orchestra
endif

#MODULES += os/services/network-energest
MODULES += os/net/app-layer/comp

MODULES_REL += ./resources

CONTIKI = ../..

include $(CONTIKI)/Makefile.include
