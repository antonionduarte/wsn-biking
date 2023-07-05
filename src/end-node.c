#include <stdio.h>

#include "stdio.h"
#include "contiki.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "config.h"
#include "dev/leds.h"
#include "sys/log.h"


#define LOG_MODULE "Client"
#define LOG_LEVEL LOG_LEVEL_INFO


static struct simple_udp_connection udp_conn;


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
}


PROCESS_THREAD(end_process, ev, data)
{
  static struct etimer timer;
	static char str[32];

	uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();
	
	simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);
	etimer_set(&timer, CLOCK_SECOND * INTERVAL_BETWEEN_MESSAGES_SECONDS);	
	snprintf(str, sizeof(str), "Hello Server");
	apply_config();

	while (1) {
		if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
			simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
		} else {
			LOG_INFO("Not reachable yet\n");
		}

		// if (led_state) {
		// 	leds_off(LEDS_RED);
		// 	leds_on(LEDS_GREEN);
		// } else {
		// 	leds_off(LEDS_GREEN);
		// 	leds_on(LEDS_RED);
		// }

		// led_state = !led_state;
		//
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		etimer_reset(&timer);
	}


	// 	while(1) {
	// 		/* Send the message with the payload here */
	// 		int payload_size = MESSAGE_SIZE - sizeof(int);
	// 		unsigned char *payload = malloc(payload_size);

	// 		for (int i = 0; i < payload_size; i++) {
	// 			payload[i] = 0;
	// 		}

	// 		message_counter++;


  PROCESS_END();
}
