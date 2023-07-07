#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "stdio.h"
#include "contiki.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "packetbuf.h"
#include "dev/leds.h"
#include "sys/log.h"


#define LOG_MODULE "Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define RPL_CONF_DAG_ROOT_RANK 1

static struct simple_udp_connection udp_conn;
static process_event_t message_received_event;

PROCESS(end_process, "End Node Process");
AUTOSTART_PROCESSES(&end_process);


static void udp_rx_callback(
	struct simple_udp_connection *conn,
	const uip_ipaddr_t *sender_addr,
	uint16_t sender_port,
	const uip_ipaddr_t *receiver_addr,
	uint16_t receiver_port,
	const uint8_t *data,
	uint16_t datalen
) {
	// TODO: Do stuff within the rx_callback
	// Save the message id, plus the timestamp of when it was received
	// On the RPI.
	
	int16_t rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
	int16_t lqi = packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY);
	
	LOG_INFO("LQI and RSSI: %d, %d\n", rssi, lqi);
	LOG_INFO("Received request '%.*s'\n", datalen, (char *) data);

	process_post(&end_process, message_received_event, NULL);
}


PROCESS_THREAD(end_process, ev, data)
{
  static struct etimer timer;
	static char str[32];

	uip_ipaddr_t relay_ipaddr; // The ip_addr of the relay node - the DAG root.

  PROCESS_BEGIN();

	simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);
	etimer_set(&timer, CLOCK_SECOND * INTERVAL_BETWEEN_MESSAGES_SECONDS);	
	snprintf(str, sizeof(str), "Hello Server");
	apply_config();

	while (1) {
		PROCESS_WAIT_EVENT();

		if (ev == message_received_event) etimer_reset(&timer);

		if (etimer_expired(&timer)) {
			if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&relay_ipaddr)) {
				simple_udp_sendto(&udp_conn, str, strlen(str), &relay_ipaddr);
			} else {
				LOG_INFO("Not reachable yet\n");
			}

			etimer_reset(&timer);
		}
	}

  PROCESS_END();
}
