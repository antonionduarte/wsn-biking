#include "stdio.h"
#include "contiki.h"
#include "net/routing/routing.h"


#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678


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
	
	// In this case, we need to check the IP_ADDR of the person who sent the message,
	// We need to get the IP_ADDR of the other node
	// And then we need to forward the message to the other node
}


PROCESS_THREAD(end_process, ev, data)
{
  static struct etimer timer;
  uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();

		/* Setup a periodic timer that expires after 10 seconds. */
		etimer_set(&timer, CLOCK_SECOND * 10);

		if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {

			// probably need to establish a UDP connection with the root here.
			// also need to make a callback function for when I receive any requests
			
			while(1) {
				printf("Hello, world\n");

				/* Wait for the periodic timer to expire and then restart the timer. */
				PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
				etimer_reset(&timer);
			}
	
		}

  PROCESS_END();
}
