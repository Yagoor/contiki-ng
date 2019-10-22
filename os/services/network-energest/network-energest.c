#include "contiki.h"
#include "contiki-net.h"
#include "sys/energest.h"
#include "network-energest.h"
#include <stdio.h>
#include <limits.h>

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "Energest"
#define LOG_LEVEL LOG_LEVEL_INFO

static unsigned long last_tx, last_rx, last_time, last_cpu, last_lpm, last_deep_lpm;
static unsigned long delta_tx, delta_rx, delta_time, delta_cpu, delta_lpm, delta_deep_lpm;
static unsigned long curr_tx, curr_rx, curr_time, curr_cpu, curr_lpm, curr_deep_lpm;
static struct uip_udp_conn *snmp_udp_conn = NULL;

PROCESS(network_energest_process, "Simple Energest");
/*---------------------------------------------------------------------------*/
static unsigned long
to_permil(unsigned long delta_metric, unsigned long delta_time)
{
  return (1000ul * (delta_metric)) / delta_time;
}
/*---------------------------------------------------------------------------*/
static void
network_energest_step(void)
{
  static unsigned count = 0;
static unsigned char packet[512];
static uint32_t packet_len;

packet_len = 0;
  energest_flush();

  curr_time = ENERGEST_GET_TOTAL_TIME();
memcpy(packet + packet_len, &curr_time, sizeof(curr_time));
packet_len += sizeof(curr_time);

  curr_cpu = energest_type_time(ENERGEST_TYPE_CPU);
  curr_lpm = energest_type_time(ENERGEST_TYPE_LPM);
  curr_deep_lpm = energest_type_time(ENERGEST_TYPE_DEEP_LPM);
  curr_tx = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  curr_rx = energest_type_time(ENERGEST_TYPE_LISTEN);

  delta_time = curr_time - last_time;
  delta_cpu = curr_cpu - last_cpu;
  delta_lpm = curr_lpm - last_lpm;
  delta_deep_lpm = curr_deep_lpm - last_deep_lpm;
  delta_tx = curr_tx - last_tx;
  delta_rx = curr_rx - last_rx;

  last_time = curr_time;
  last_cpu = curr_cpu;
  last_lpm = curr_lpm;
  last_deep_lpm = curr_deep_lpm;
  last_tx = curr_tx;
  last_rx = curr_rx;

uip_udp_packet_sendto(snmp_udp_conn, packet, packet_len, &UIP_IP_BUF->srcipaddr, UIP_UDP_BUF->srcport);

  LOG_INFO("--- Period summary #%u (%lu seconds)\n", count++, delta_time/ENERGEST_SECOND);
  LOG_INFO("Total time  : %10lu\n", delta_time);
  LOG_INFO("CPU         : %10lu/%10lu (%lu permil)\n", delta_cpu, delta_time, to_permil(delta_cpu, delta_time));
  LOG_INFO("LPM         : %10lu/%10lu (%lu permil)\n", delta_lpm, delta_time, to_permil(delta_lpm, delta_time));
  LOG_INFO("Deep LPM    : %10lu/%10lu (%lu permil)\n", delta_deep_lpm, delta_time, to_permil(delta_deep_lpm, delta_time));
  LOG_INFO("Radio Tx    : %10lu/%10lu (%lu permil)\n", delta_tx, delta_time, to_permil(delta_tx, delta_time));
  LOG_INFO("Radio Rx    : %10lu/%10lu (%lu permil)\n", delta_rx, delta_time, to_permil(delta_rx, delta_time));
  LOG_INFO("Radio total : %10lu/%10lu (%lu permil)\n", delta_tx+delta_rx, delta_time, to_permil(delta_tx+delta_rx, delta_time));
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(network_energest_process, ev, data)
{
  PROCESS_BEGIN();

  /* new connection with remote host */
  snmp_udp_conn = udp_new(NULL, 0, NULL);
  udp_bind(snmp_udp_conn, UIP_HTONS(30001));
  LOG_DBG("Listening on port %u\n", uip_ntohs(snmp_udp_conn->lport));

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
  last_time = ENERGEST_GET_TOTAL_TIME();
  last_cpu = energest_type_time(ENERGEST_TYPE_CPU);
  last_lpm = energest_type_time(ENERGEST_TYPE_LPM);
  curr_tx = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  last_deep_lpm = energest_type_time(ENERGEST_TYPE_DEEP_LPM);
  last_rx = energest_type_time(ENERGEST_TYPE_LISTEN);
  process_start(&network_energest_process, NULL);
}

/** @} */
