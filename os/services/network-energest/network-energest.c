#include "contiki.h"
#include "contiki-net.h"
#include "sys/energest.h"
#include "network-energest.h"
#include <limits.h>

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "Energest"
#define LOG_LEVEL LOG_LEVEL_NONE

static struct uip_udp_conn *snmp_udp_conn = NULL;

PROCESS(network_energest_process, "Network Energest");
/*---------------------------------------------------------------------------*/
static void
network_energest_step(void)
{
  static unsigned char packet[512];
  static uint32_t packet_len;

  packet_len = 0;
  energest_flush();

#define ADD_VALUE(name, value) \
  uint32_t name; \
  uint32_t name##_real; \
  name##_real = value; \
  name = UIP_HTONL(name##_real); \
  memcpy(packet + packet_len, &name, sizeof(uint32_t)); \
  packet_len += sizeof(uint32_t); \

  ADD_VALUE(system, ENERGEST_CURRENT_TIME());

  ADD_VALUE(total, ENERGEST_GET_TOTAL_TIME());

  ADD_VALUE(total_cpu, energest_type_time(ENERGEST_TYPE_CPU));

  ADD_VALUE(total_lpm, energest_type_time(ENERGEST_TYPE_LPM));

  ADD_VALUE(total_deep_lpm, energest_type_time(ENERGEST_TYPE_DEEP_LPM));

  ADD_VALUE(total_transmit, energest_type_time(ENERGEST_TYPE_TRANSMIT));

  ADD_VALUE(total_listen, energest_type_time(ENERGEST_TYPE_LISTEN));

  ADD_VALUE(seconds, ENERGEST_SECOND);

  uip_udp_packet_sendto(snmp_udp_conn, packet, packet_len, &UIP_IP_BUF->srcipaddr, UIP_UDP_BUF->srcport);

  LOG_INFO("--- Period summary #\n");
  LOG_INFO("System time : %lu\n", (long unsigned int)(system_real));
  LOG_INFO("Total time  : %lu\n", (long unsigned int)(total_real));
  LOG_INFO("CPU         : %lu\n", (long unsigned int)total_cpu_real);
  LOG_INFO("LPM         : %lu\n", (long unsigned int)total_lpm_real);
  LOG_INFO("Deep LPM    : %lu\n", (long unsigned int)total_deep_lpm_real);
  LOG_INFO("Radio Tx    : %lu\n", (long unsigned int)total_transmit_real);
  LOG_INFO("Radio Rx    : %lu\n", (long unsigned int)total_listen_real);
  LOG_INFO("Seconds     : %lu\n", (long unsigned int)seconds_real);
  /* LOG_INFO("Radio total : % 10lu / % 10lu (% lu permil) \ n ", delta_tx + delta_rx, delta_time, to_permil(delta_tx + delta_rx, delta_time)); */
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(network_energest_process, ev, data)
{
  PROCESS_BEGIN();

  /* new connection with remote host */
  snmp_udp_conn = udp_new(NULL, 0, NULL);
  udp_bind(snmp_udp_conn, UIP_HTONS(30001));
  LOG_INFO("Listening on port %u\n", uip_ntohs(snmp_udp_conn->lport));

  while(1) {
    PROCESS_YIELD();

    if(ev == tcpip_event) {
      if(uip_newdata()) {
        network_energest_step();
      }
    }
  } /* while (1) */

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
network_energest_init(void)
{
  energest_flush();
  process_start(&network_energest_process, NULL);
}
/** @} */
