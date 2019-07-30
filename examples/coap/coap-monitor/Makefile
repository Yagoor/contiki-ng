CONTIKI_PROJECT = coap-example-server 
all: $(CONTIKI_PROJECT)

# Do not try to build on Sky because of code size limitation
PLATFORMS_EXCLUDE = sky z1

TARGET=cc26x0-cc13x0

MAKE_MAC = MAKE_MAC_TSCH

# Include the CoAP implementation
MODULES += os/net/app-layer/coap
MODULES += os/services/orchestra
MODULES += os/services/simple-energest

# Include CoAP resources
MODULES_REL += ./resources

CONTIKI=../../..
include $(CONTIKI)/Makefile.include
